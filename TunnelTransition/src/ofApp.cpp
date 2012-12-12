#include "ofApp.h"

float ofApp::_(string name) {
	return gui.getValueF(name);
}

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	midi.openPort("IAC Driver Bus 1");
	
	gui.setup(250, 900);
	gui.addPanel("Settings");
	gui.addToggle("randomize", false);
	gui.addToggle("export", false);
	gui.addSlider("tunnelLength", tunnel.tunnelLength, 0, 512, true);
	gui.addSlider("circleResolution", tunnel.circleResolution, 1, 30, true);
	gui.addSlider("segmentSubdivision", tunnel.segmentSubdivision, 1, 12, true);
	gui.addSlider("baseBlend", tunnel.baseBlend, 0, 1);
	gui.addSlider("dissolveBlend", tunnel.dissolveBlend, 0, 1);
	gui.addSlider("pulseBlend", tunnel.pulseBlend, 0, 1);
	gui.addSlider("fogNear", tunnel.fogNear, 0, 1000);
	gui.addSlider("fogFar", tunnel.fogFar, 0, 4000);
	gui.addSlider("tunnelSeparation", tunnel.tunnelSeparation, 10, 1000);
	gui.addSlider("tunnelRadius", tunnel.tunnelRadius, 100, 1000);
	gui.addSlider("rotationChange", tunnel.rotationChange, 0, .1);
	gui.addSlider("rotationAmount", tunnel.rotationAmount, 0, 45);
	gui.addSlider("moveSpeed", tunnel.moveSpeed, 0, 5000);
	gui.addSlider("lerpViewRate", tunnel.lerpViewRate, 0, .1);
	gui.addSlider("segmentTiming", tunnel.segmentTiming, 0, 1);
	gui.addSlider("randomDissolve", tunnel.randomDissolve, 0, 10);
	gui.addToggle("useTriangles", tunnel.useTriangles);
	
	tunnel.setup();
}

void ofApp::update() {
	if(gui.hasValueChangedInPanel("Settings")) {
		if(_("export")) {
			gui.setValueB("export", false);
			tunnel.save("tunnel.ply");
		}
		tunnel.tunnelLength = _("tunnelLength");
		tunnel.circleResolution = _("circleResolution");
		tunnel.segmentSubdivision = _("segmentSubdivision");
		tunnel.baseBlend = _("baseBlend");
		tunnel.dissolveBlend = _("dissolveBlend");
		tunnel.pulseBlend = _("pulseBlend");
		tunnel.fogNear = _("fogNear");
		tunnel.fogFar = _("fogFar");
		tunnel.tunnelSeparation = _("tunnelSeparation");
		tunnel.tunnelRadius = _("tunnelRadius");
		tunnel.rotationChange = _("rotationChange");
		tunnel.rotationAmount = _("rotationAmount");
		tunnel.moveSpeed = _("moveSpeed");
		tunnel.lerpViewRate = _("lerpViewRate");
		tunnel.segmentTiming = _("segmentTiming");
		tunnel.randomDissolve = _("randomDissolve");
		tunnel.useTriangles = _("useTriangles");
		if(_("randomize")) {
			gui.setValueB("randomize", false);
			tunnel.randomize();
		}
		tunnel.setup();
		gui.clearAllChanged();
	}
	tunnel.update(ofGetElapsedTimef());
	if(tunnel.isSegmentNew()) {
		midi.sendNoteOn(1, 64, 127);
		midi.sendNoteOff(1, 64);
	}
}

void ofApp::draw() {
	ofBackground(0);
	cam.begin();
	tunnel.draw();
	cam.end();
}
