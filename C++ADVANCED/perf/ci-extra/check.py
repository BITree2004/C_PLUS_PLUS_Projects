import json
import sys

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Wrong argument count: ", len(sys.argv) != 4)
        sys.exit(1)

    json_fix = None
    json_base = None
    with open(sys.argv[1]) as fix:
        json_fix = json.load(fix)

    with open(sys.argv[2]) as base:
        json_base = json.load(base)

    solution_time = json_fix["benchmarks"][0]["real_time"]
    base_time = json_base["benchmarks"][0]["real_time"]
    expected_multiplier = float(sys.argv[3])
    actual_multiplier = float(base_time) / float(solution_time)

    print(f"solution time: {solution_time}")
    print(f"base time: {base_time}")
    print(f"expected improvement multiplier: {expected_multiplier}")
    print(f"actual improvement multiplier: {actual_multiplier}")

    if actual_multiplier >= expected_multiplier:
        print("OK")
    else:
        print("FAIL")
        sys.exit(1)
