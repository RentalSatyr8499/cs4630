import os
import subprocess
import sys

def runTests(msg, path, executable):
    print("\n" + msg)
    print("-" * len(msg))

    # Ensure directory exists
    if not os.path.isdir(path):
        print(f"[ERROR] Directory not found: {path}")
        return

    # List files in the directory
    files = sorted(os.listdir(path))
    if not files:
        print(f"[INFO] No files found in {path}")
        return

    for fname in files:
        fpath = os.path.join(path, fname)

        # Skip subdirectories
        if not os.path.isfile(fpath):
            continue

        # Run the scanner executable with the file piped into stdin
        try:
            with open(fpath, "rb") as f:
                result = subprocess.run(
                    [executable],
                    stdin=f,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE
                )
        except Exception as e:
            print(f"[ERROR] Could not run on {fname}: {e}")
            continue

        # Print results
        if result.stdout.strip():
            print(f"File: {fname}. Output: {result.stdout.strip()}")
        else:
            print(f"File: {fname}. No output")
        if result.stderr.strip():
            print("Errors:")
            print(result.stderr.strip())


import subprocess
import sys

def main():
    # Determine which make target to run
    target = "2"   # default
    if len(sys.argv) > 1:
        if sys.argv[1] in ("1", "2"):
            target = sys.argv[1]
        else:
            print(f"[ERROR] Unknown target '{sys.argv[1]}'. Use 1 or 2.")
            sys.exit(1)

    print(f"Running make {target}...")
    make = subprocess.run(
        ["make", target],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    print(make.stdout)
    if make.returncode != 0:
        print("[ERROR] make failed:")
        print(make.stderr)
        sys.exit(1)

    executable = "./scanner"
    if not os.path.isfile(executable):
        print(f"[ERROR] Expected executable '{executable}' not found after make")
        sys.exit(1)

    paths = [
        ["Running against trivial encrypted virus files...", './samples/e/trivial/'],
        ["Running against actual encrypted virus files...", './samples/e/actual/'],
        ["Running against false positive file...", './samples/false-pos/'],
        ["Running against random no positive files...", './samples/n/random/'],
        ["Running against actual no positive files...", './samples/n/actual/'],
        ["Running against trivial tricky jump files...", './samples/t/trivial/'],
        ["Running against actual tricky jump files...", './samples/t/actual/']
    ]

    for msg, path in paths:
        runTests(msg, path, executable)



if __name__ == "__main__":
    main()
