#!/bin/bash
WORK_DIR=$(pwd | sed -e "s/\/out//g")
echo "${WORK_DIR}"

rm ./coverage.info
rm -rf ./coverage
lcov -b . -c -d . -o ./coverage.info
lcov -r ./coverage.info '/usr/*' "${WORK_DIR}/core/persistence/*" "${WORK_DIR}/core/utilities/json.hpp" "${WORK_DIR}/out/*" "${WORK_DIR}/tests/*" -o ./coverage.info
genhtml -o ./coverage ./coverage.info
