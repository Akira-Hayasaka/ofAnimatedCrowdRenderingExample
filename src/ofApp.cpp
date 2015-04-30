#include "ofApp.h"

void ofApp::setup()
{
    ofEnableDepthTest();
    ofSetFrameRate(60);
    ofBackground(ofColor::white);
    
    model.setup();
}

void ofApp::update()
{
    Globals::ELAPSED_TIME = ofGetElapsedTimef();
    
    model.update();
}

void ofApp::draw()
{
    float t = Globals::ELAPSED_TIME;
    
    ofVec3f camPos;
    camPos.x = cos(t * 13 * DEG_TO_RAD) * 100;
    camPos.z = sin(t * 13 * DEG_TO_RAD) * 100;
    cam.setPosition(camPos);
    cam.lookAt(ofVec3f::zero(), ofVec3f(0, 1, 0));
    cam.begin();
    
    model.draw(cam.getModelViewProjectionMatrix());
    
    cam.end();
    
    ofDrawBitmapStringHighlight("fps:" + ofToString(ofGetFrameRate()), 10, ofGetHeight() - 20);
}

void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}