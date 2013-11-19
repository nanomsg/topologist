ADD_TEST(test_twofold_1
    ${TOPOLOGY}
    --config ${CMAKE_CURRENT_SOURCE_DIR}/twofold.yaml
    --query-file ${CMAKE_CURRENT_SOURCE_DIR}/queries.txt
    --output ${CMAKE_CURRENT_BINARY_DIR}/results.out)
ADD_TEST(test_twofold_1_check
    ${DIFF} -u ${CMAKE_CURRENT_SOURCE_DIR}/results.txt
               ${CMAKE_CURRENT_BINARY_DIR}/results.out)
