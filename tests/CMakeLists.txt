include("../cmake/GoogleTest.cmake")

add_executable(3dconv_test
test.cpp
)

target_compile_features(3dconv_test PUBLIC cxx_std_20)

target_link_libraries(3dconv_test gtest FA3dConverter)
