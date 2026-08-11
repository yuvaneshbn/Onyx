include("C:/Users/YUVANESH/Downloads/Onyx/build/Desktop_Qt_6_11_1_MSVC2022_64bit_Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/onyx-voip-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtdeclarative")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Users/YUVANESH/Downloads/Onyx/build/Desktop_Qt_6_11_1_MSVC2022_64bit_Debug/onyx-voip.exe"
    GENERATE_QT_CONF
)
