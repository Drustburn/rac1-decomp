#!/usr/bin/env python3
"""
Version-specific C without a model: the shared C with the numbers this version changed.

  bash tools/docker/run.sh python tools/version_clone.py --version jp [FUNC ...] [-P 12]

For each function whose shared C builds the primary's bytes but not this version's because a
constant differs (tools/version_check.py status `differs`, config/versions/<K>/map.tsv), the
version's instructions are compared with the primary's: when they differ only in immediates (a
frame count of 50 against 60, a field offset), every literal of the shared C's definition that
spells an old value is replaced by the new one (tools/overlay_variants.py's number_changes() and
candidates(), which clone level variants the same way), and each candidate is checked by
tools/version_src.py try. An EXACT one is written as the function's version-specific definition
(src/versions/<K>/...). Functions that differ in more than numbers are left for people (or
worker agents) and listed.
"""
import argparse
import os
import struct
import sys
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
os.chdir(ROOT)
sys.path.insert(0, str(ROOT / "tools"))
import versions  # noqa: E402


def words(b):
    return struct.unpack("<%dI" % (len(b) // 4), b[:len(b) // 4 * 4])


def target_path(v, name, src):
    return ROOT / "src/versions" / v.key / str(Path(src))[len("src/"):-2] / f"{name}.c"


def one(key, name):
    import overlay_variants as ovv
    import try_func
    import version_check as vc
    import version_src
    v = versions.get(key)
    e = vc.entries_by_name(v)[name]
    mine = sorted(e["places"], key=lambda p: (-1 if p[0] == "boot" else p[0], p[1]))
    pp = vc.primary_places()[name]
    sp, xa = next((p for p in mine if p[0] in {q[0] for q in pp}), mine[0])
    pa = sorted(a for s, a in pp if s == sp)
    if not pa:
        return name, "skipped: no place in the same program"
    size = vc.primary_sizes()[name]
    if size != e["size"]:
        return name, f"skipped: size {size} here {e['size']}"
    a = words(vc.code_bytes(versions.PRIMARY, sp, pa[0], size))
    b = words(vc.code_bytes(v, sp, xa, size))
    changes = ovv.number_changes(a, b)
    if changes is None:
        return name, "skipped: differs in more than numbers"
    seg, src, first, last = try_func.find_stub(name)
    body = "\n".join(Path(src).read_text(errors="replace").splitlines()[first:last + 1])
    work = ROOT / "build-sn/versions" / v.key / "clone" / name
    work.mkdir(parents=True, exist_ok=True)
    verdict = "no literal for the changed numbers"
    for k, cand in enumerate(ovv.candidates(body, changes)):
        if cand == body:
            continue
        path = work / f"clone{k}.c"
        path.write_text(cand + "\n")
        verdict = version_src.try_one(v, name, path)
        if verdict.startswith("EXACT"):
            out = target_path(v, name, src)
            out.parent.mkdir(parents=True, exist_ok=True)
            out.write_text(cand + "\n")
            return name, verdict + f" -> {out.relative_to(ROOT)}"
    return name, verdict


_SLOT = None


def worker_prefix(slot, prefixes):
    """A worker's own Wine prefix (RAC1_WINEPREFIXES, separated by ':'): one wineserver per
    prefix serialises its compiles, so parallel workers each take one."""
    with slot.get_lock():
        i = slot.value
        slot.value += 1
    os.environ["RAC1_WINEPREFIX"] = prefixes[i % len(prefixes)]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("names", nargs="*")
    ap.add_argument("--version", required=True)
    ap.add_argument("-P", "--parallel", type=int, default=os.cpu_count() or 4)
    a = ap.parse_args()
    v = versions.get(a.version)
    import version_check as vc
    import version_src
    have = version_src.overrides(v)
    todo = a.names or [n for n, r in vc.read_map(v).items()
                       if r["status"] == "differs" and r["source"].startswith("src/") and n not in have]
    from toolchain import start_wineserver
    start_wineserver()
    exact = 0
    import multiprocessing
    prefixes = [p for p in os.environ.get("RAC1_WINEPREFIXES", "").split(":") if p]
    kw = {}
    if prefixes:
        kw = {"initializer": worker_prefix, "initargs": (multiprocessing.Value("i", 0), prefixes)}
    with ProcessPoolExecutor(a.parallel, **kw) as ex:
        for name, verdict in ex.map(one, [v.key] * len(todo), todo):
            print(f"{name}: {verdict}", flush=True)
            exact += verdict.startswith("EXACT")
    print(f"{v.key}: {exact} of {len(todo)} cloned exactly")


if __name__ == "__main__":
    main()
