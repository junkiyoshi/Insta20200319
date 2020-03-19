#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofSetLineWidth(3);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
	this->fbo.readToPixels(this->pixels);
	this->pixels_mat = cv::Mat(this->pixels.getHeight(), this->pixels.getWidth(), CV_8UC4, this->pixels.getData());
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->location_list.clear();
	this->color_list.clear();

	this->fbo.begin();
	ofClear(0);
	ofFill();

	ofColor color;
	glm::vec2 location;
	for (int i = 0; i < 16; i++) {

		color.setHsb(ofMap(i, 0, 16, 0, 255), 255, 255);
		auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));

		ofSetColor(color);
		for (int k = 0; k < 8; k++) {

			location = glm::vec2(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + k) * 0.008), 0, 1, 0, ofGetWidth()), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + k) * 0.008), 0, 1, 0, ofGetHeight()));

			ofBeginShape();
			for (int deg = 0; deg < 360; deg++) {

				ofVertex(location + glm::vec2(ofMap(k, 0, 8, 5, 40) * cos(deg * DEG_TO_RAD), ofMap(k, 0, 8, 5, 40) * sin(deg * DEG_TO_RAD)));
			}
			ofEndShape(true);
		}

		this->color_list.push_back(color);
		this->location_list.push_back(location);
	}

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	cv::GaussianBlur(this->pixels_mat, this->pixels_mat, cv::Size(49, 49), 50, 50);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofFill();
	ofSetColor(255);
	ofImage draw_image;
	draw_image.setFromPixels(this->pixels);
	draw_image.draw(0, 0);
	
	for (int i = 0; i < this->location_list.size(); i++) {

		vector<glm::vec2> vertices;
		for (int deg = 0; deg < 360; deg++) {

			vertices.push_back(this->location_list[i] + glm::vec2(15 * cos(deg * DEG_TO_RAD), 15 * sin(deg * DEG_TO_RAD)));
		}

		ofFill();
		ofSetColor(255);
		ofBeginShape();
		ofVertices(vertices);
		ofEndShape(true);

		ofNoFill();
		ofSetColor(this->color_list[i]);
		ofBeginShape();
		ofVertices(vertices);
		ofEndShape(true);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}