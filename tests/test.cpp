/**
 * @file test.cpp
 * @author F. Abrignani (federignoli@hotmail.it)
 * @brief test.
 * @version 0.1
 * @date 2024-06-29
 * @private
 * @copyright Copyright (c) 2024 Federico Abrignani (federignoli@hotmail.it).
 * 
 */
#include <gtest/gtest.h>
#include <FAConverter.hpp>


TEST(CompileTime, Test) {
    /*
    As more models are added we should uncomment the following tests and
    add the corrisponding models to this test.
    It is expected to fail on a static_assert if the model or operation is not supported.
    For now we only test the OBJ model existance and we write an untranformed stl file.
    */

    FAConverter::Model<FAConverter::FileType::OBJ> objModel;
    objModel.read("/home/federico/Scrivania/objFiles/airboat.obj");
    objModel.write<FAConverter::FileType::STL>("example.stl");
    //objModel.write<FAConverter::FileType::OBJ>("example.obj"); // obj writing not supported yet should fail
    //FAConverter::Model<FAConverter::FileType::STL> stlModel; // stl model not supported yet should fail
    //stlModel.read("example.stl");
    //stlModel.write<FAConverter::FileType::OBJ>("example.obj");

    ASSERT_TRUE(true);
}

TEST(OBJModel, ApplyTransform) {
    /*
    This test is used to test the transformation of the model.
    It is expected to fail on a compile time error if the model is not supported.
    */
    FAConverter::Model<FAConverter::FileType::OBJ> objModel;
    objModel.read("/home/federico/Scrivania/objFiles/airboat.obj");

    FAConverter::Matrix4x4 transform = FAConverter::Matrix4x4::translation(10.0f, 5.0f, 3.0f) *
                                       FAConverter::Matrix4x4::rotationZ(45.0f) *
                                       FAConverter::Matrix4x4::rotationY(45.0f) *
                                       FAConverter::Matrix4x4::rotationX(45.0f) *
                                       FAConverter::Matrix4x4::scaling(2.0f, 2.0f, 2.0f);

    objModel.applyTransform(transform);

    objModel.write<FAConverter::FileType::STL>("transformed_example.stl");

    ASSERT_TRUE(true);
}

TEST(OBJModel, PointInsideCube) {
    /*
    This test is used to test the isPointInside method of the OBJ model.
    */
    FAConverter::Model<FAConverter::FileType::OBJ> objModel;
    objModel.read("cube.obj");

    FAConverter::Vertex insidePoint = {0.2f, 0.2f, 0.2f, 0.0f};
    FAConverter::Vertex outsidePoint = {1.5f, 1.5f, 1.5f, 0.0f};

    auto answerInsidePoint = objModel.isPointInside(insidePoint);
    auto answerOutsidePoint = objModel.isPointInside(outsidePoint);

    ASSERT_TRUE(answerInsidePoint);
    ASSERT_FALSE(answerOutsidePoint);
}

TEST(OBJModel, PointInsideCubeWithinCube) {
    /*
    This test is used to test the isPointInside method of the OBJ model.
    */
    FAConverter::Model<FAConverter::FileType::OBJ> objModel;
    // the cucube.obj file has a cube within a cube both centered around 0,0,0.
    objModel.read("cucube.obj");

    FAConverter::Vertex outsidePoint = {0.0f, 0.0f, 0.0f, 0.0f};

    auto answerOutsidePoint = objModel.isPointInside(outsidePoint);

    ASSERT_FALSE(answerOutsidePoint);
}

TEST(OBJModel, CalculateSurfaceArea) {
    /*
    This test is used to test the calculateSurfaceArea method of the OBJ model.
    */
    FAConverter::Model<FAConverter::FileType::OBJ> objModel;
    objModel.read("cube.obj");

    float surfaceArea = objModel.calculateSurfaceArea();

    float expectedSurfaceArea = 4.86f; // 0.9^2 * 6 = 4.86
    float tolerance = 0.001f;

    ASSERT_NEAR(surfaceArea, expectedSurfaceArea, tolerance);
}

TEST(OBJModel, CalculateVolume) {
    FAConverter::Model<FAConverter::FileType::OBJ> objModel;
    objModel.read("cucube.obj");

    float volume = objModel.calculateVolume();

    float expectedVolume = 0.784f;
    float tolerance = 0.001f;
    ASSERT_NEAR(volume, expectedVolume, tolerance);
}

int main(int argc, char* argv[]) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
