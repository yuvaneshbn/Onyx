include("C:/Users/YUVANESH/Downloads/Onyx/build-ninja/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/onyx-voip-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtdeclarative")

qt6_deploy_qml_imports(TARGET onyx-voip PLUGINS_FOUND plugins_found)
qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Users/YUVANESH/Downloads/Onyx/build-ninja/onyx-voip.exe"
    ADDITIONAL_MODULES ${plugins_found}
    GENERATE_QT_CONF
)