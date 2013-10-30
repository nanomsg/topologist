ADD_TEST(test_simple_1
    ${TOPOLOGY}
    --config ${CMAKE_CURRENT_SOURCE_DIR}/simple.yaml
    --query-file ${CMAKE_CURRENT_SOURCE_DIR}/queries.txt
    --output ${CMAKE_CURRENT_BINARY_DIR}/results.out)
ADD_TEST(test_simple_1_check
    ${DIFF} -u ${CMAKE_CURRENT_SOURCE_DIR}/results.txt
               ${CMAKE_CURRENT_BINARY_DIR}/results.out)
