#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

int main(){
    
//    ofGLWindowSettings settings;
//    //settings.setSize(1024, 768);
//    settings.setSize(1280, 960);
//    ofCreateWindow(settings);
//    return ofRunApp(std::make_shared<ofApp>());
    
    ofGLFWWindowSettings settings;
    settings.setSize(1024, 768);
    settings.setPosition(glm::vec2(300,0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    settings.setSize(1920, 1080);
    settings.setPosition(glm::vec2(0,0));
    settings.resizable = true;
    // uncomment next line to share main's OpenGL resources with gui
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> topWindow = ofCreateWindow(settings);
    topWindow->setVerticalSync(false);

    shared_ptr<ofApp> mainApp(new ofApp);
    mainApp->setupTop();
    ofAddListener(topWindow->events().draw,mainApp.get(),&ofApp::drawTop);

    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
    
}
