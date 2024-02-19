#!/bin/bash
set -euo pipefail
IFS=$' \t\n'

cmake-build-$1/perf/json --benchmark_format=json >json_fix.json
cmake-build-$1/perf/jpeg --benchmark_format=json >jpeg_fix.json
cmake-build-$1/perf/logger --benchmark_format=json >logger_fix.json
cmake-build-$1/perf/pi --benchmark_format=json >pi_fix.json
cmake-build-$1/perf/radix_sort --benchmark_format=json >sort_fix.json

cmake-build-$1/ci-extra/perf/base_json --benchmark_format=json >json_base.json
cmake-build-$1/ci-extra/perf/base_jpeg --benchmark_format=json >jpeg_base.json
cmake-build-$1/ci-extra/perf/base_logger --benchmark_format=json >logger_base.json
cmake-build-$1/ci-extra/perf/base_pi --benchmark_format=json >pi_base.json
cmake-build-$1/ci-extra/perf/base_radix_sort --benchmark_format=json >sort_base.json

set +e
fail=0

echo "Testing json..."
python3 ci-extra/check.py json_fix.json json_base.json 1.5 || ((fail = 1))
echo "Testing jpeg..."
python3 ci-extra/check.py jpeg_fix.json jpeg_base.json 4 || ((fail = 1))
echo "Testing logger..."
python3 ci-extra/check.py logger_fix.json logger_base.json 30 || ((fail = 1))
echo "Testing pi..."
python3 ci-extra/check.py pi_fix.json pi_base.json 40 || ((fail = 1))
echo "Testing sort..."
python3 ci-extra/check.py sort_fix.json sort_base.json 1.2 || ((fail = 1))

exit $fail
