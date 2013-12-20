#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	gameStatus = 1; // 0 = start screen
	// 1 = playing
	// 2 = game over
	
	bg.loadImage("images/background.png");
	
	splatSound.loadSound("audio/splatter.mp3");
	
//	faceMiss.loadImage("images/face2.png");
//	faceHit.loadImage("images/face1.png");
	
	target.x = 200;
	target.y = 200;
	target.z = 0;
	hasHit = false;
	
	currentSplat = floor((ofRandom(7))); //floor will give integer btwn 0-6, select this line for every time you select your food
	for (int i=0; i<7; i++) {
		
		string splatter = "images/splat" + ofToString(i) + ".png";
		splat[i].loadImage(splatter);
	}
	
	//	isThrowing = false;
	myBall.init();
    
	//	ofEnablerightBlendMode(ofBlendMode ADD); // this affects png
    ofSetLogLevel(OF_LOG_VERBOSE);
	
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(2);
    openNIDevice.start();
    
    // set properties for all user masks and point clouds
    // openNIDevice.setUseMaskPixelsAllUsers(true); // if you just want pixels, use this set to true
    openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
    openNIDevice.setPointCloudResolutionAllUsers(2); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
    
    // you can alternatively create a 'base' user class
	//    ofxOpenNIUser user;
	//    user.setUseMaskTexture(true);
	//    user.setUsePointCloud(true);
	//    user.setPointCloudDrawSize(2);
	//    user.setPointCloudResolution(2);
	//    openNIDevice.setBaseUserClass(user);
	
    verdana.loadFont(ofToDataPath("verdana.ttf"), 96);
	
	//requiredScore = 10;
	
}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
	
	ofSoundUpdate();
	
	long currTime = ofGetElapsedTimeMillis();
	int min = 60000;
	
	//playing = false; //uncomment to see gameover screen
	
	for (int i = 0; i < splatList.size(); i++ ) {
		splatList[i].update();
	}
	
	
	if(hasHit && gameStatus == 1){ //only count hits if we're playing
		score += 1;
		
		target.x = ofRandom(600);
		target.y = ofRandom(400);
		hasHit = false;
		
		
		
		
		//		if (currTime > min && currTime < min*2){ //1  -  2 min
		//			if (score < 10){
		//				playing = false;
		//			}
		//		}
		//		
		//		else if (currTime >= min*2 && currTime < min*3){ //2 - 3 min
		//			if (score < 20){
		//				playing = false;
		//			}
		//		}
		//		
		//		else if (currTime >= min*3 && currTime < min*4){ //2 - 3 min
		//			if (score < 30){
		//				playing = false;
		//			}
		//		}
		
		
	}
	
	//if ( currTime % min < 15 ) {  //how well is this working? sometimes modulos get weird
//		if ( score < requiredScore ) {
//			gameStatus = 2; // GAME OVER
//		}
//		requiredScore += 10;
//		
//		cout << requiredScore << endl;
//	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	ofSetColor(ofColor::lightGreen);
	
	ofPushMatrix();
	ofScale(ofGetWidth()/640, ofGetHeight()/480);
	// draw debug (ie., image, depth, skeleton)
	// openNIDevice.drawDebug();
	
	// use a blend mode so we can see 'through' the mask(s)
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	// get number of current users
	int numUsers = openNIDevice.getNumTrackedUsers();
	
	// iterate through users
	for (int i = 0; i < numUsers; i++){
		
		// get a reference to this user
		ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
		
		rightHand = user.getJoint(JOINT_RIGHT_HAND).getProjectivePosition();
		
		leftHand = user.getJoint(JOINT_LEFT_HAND).getProjectivePosition();
		
		// for body
		head = user.getJoint(JOINT_HEAD).getProjectivePosition();
		rightShoulder = user.getJoint(JOINT_RIGHT_SHOULDER).getProjectivePosition();
		leftShoulder = user.getJoint(JOINT_LEFT_SHOULDER).getProjectivePosition();
		rightElbow = user.getJoint(JOINT_RIGHT_ELBOW).getProjectivePosition();
		leftElbow = user.getJoint(JOINT_LEFT_ELBOW).getProjectivePosition();
		torsoJoint = user.getJoint(JOINT_TORSO).getProjectivePosition();
		rightHip = user.getJoint(JOINT_RIGHT_HIP).getProjectivePosition();
		leftHip = user.getJoint(JOINT_LEFT_HIP).getProjectivePosition();
		rightKnee = user.getJoint(JOINT_RIGHT_KNEE).getProjectivePosition();
		leftKnee = user.getJoint(JOINT_LEFT_KNEE).getProjectivePosition();
		rightFoot = user.getJoint(JOINT_RIGHT_FOOT).getProjectivePosition();
		leftFoot = user.getJoint(JOINT_LEFT_FOOT).getProjectivePosition();
		
				
		vel_right = rightHand - prev_rightHand;
		vel_left = leftHand - prev_leftHand;
		
		//		if (vel.length() > 50 && rightHand.z < prev_rightHand.z) {
		//			isThrowing = true;
		//
		//		}else if (vel.length() < 10) {
		//			isThrowing = false;
		//		}
		
		// draw bg
		bg.draw(-200, 0);
	
		
		// draw the silhouette
		

		if(gameStatus == 1) 
			
			//draw head
			ofCircle(head.x, head.y, 45);
			
			//draw shoulders
			//ofBeginShape();
			int shoulderLength =  ofDist(rightShoulder.x, rightShoulder.y, leftShoulder.x, leftShoulder.y);
			int shoulder_width = shoulderLength/10;
			//ofVertex(rightShoulder.x, (rightShoulder.y - shoulder_width));
//			ofVertex(rightShoulder.x, (rightShoulder.y + shoulder_width));
//			ofVertex(leftShoulder.x, (leftShoulder.y + shoulder_width));
//			ofVertex(leftShoulder.x, (leftShoulder.y - shoulder_width));
//			ofEndShape();
		
//			int shoulderLength =  ofDist(rightShoulder.x, rightShoulder.y, leftShoulder.x, leftShoulder.y);
//			ofRect(rightShoulder.x, rightShoulder.y, -(shoulderLength), (shoulderLength/3));
		
			//draw upper right arm
			ofBeginShape();
			int upperArmLength =  ofDist(rightShoulder.x, rightShoulder.y, rightElbow.x, rightElbow.y);
			int upperArm_width = upperArmLength/8;
			ofVertex((rightShoulder.x + upperArm_width), rightShoulder.y);
			ofVertex((rightShoulder.x + upperArm_width)-10, rightShoulder.y-10);
			ofVertex((rightShoulder.x + upperArm_width)-20, rightShoulder.y-20);
			ofVertex((rightShoulder.x - upperArm_width), rightShoulder.y);
			ofVertex((rightElbow.x - upperArm_width), rightElbow.y);
			ofVertex((rightElbow.x + upperArm_width), rightElbow.y);
			ofEndShape();
		
			//draw upper left arm
			ofBeginShape();
			ofVertex((leftShoulder.x - upperArm_width), leftShoulder.y);
			ofVertex((leftShoulder.x - upperArm_width)+20, leftShoulder.y-20);
			ofVertex((leftShoulder.x - upperArm_width)+10, leftShoulder.y-10);
			ofVertex((leftShoulder.x + upperArm_width), leftShoulder.y);
			ofVertex((leftElbow.x + upperArm_width), leftElbow.y);
			ofVertex((leftElbow.x - upperArm_width), leftElbow.y);
			ofEndShape();
		
			//draw lower right arm
			ofBeginShape();
			int lowerArmLength =  ofDist(rightElbow.x, rightElbow.y, rightHand.x, rightHand.y);
			int lowerArm_width = lowerArmLength/9;
			ofVertex((rightElbow.x + lowerArm_width), rightElbow.y);
			ofVertex((rightElbow.x - lowerArm_width), rightElbow.y);
			ofVertex((rightHand.x - lowerArm_width), rightHand.y);
			ofVertex((rightHand.x + lowerArm_width), rightHand.y);
			ofEndShape();
		
			//draw lower left arm
			ofBeginShape();
			ofVertex((leftElbow.x + lowerArm_width), leftElbow.y);
			ofVertex((leftElbow.x - lowerArm_width), leftElbow.y);
			ofVertex((leftHand.x - lowerArm_width), leftHand.y);
			ofVertex((leftHand.x + lowerArm_width), leftHand.y);
			ofEndShape();
		
			//draw hands
			ofCircle(rightHand.x, rightHand.y, 15);
			ofCircle(leftHand.x, leftHand.y, 15);
		
			//draw torso
			ofBeginShape();
			//shoulders
			ofVertex(rightShoulder.x, rightShoulder.y);
			ofVertex(rightShoulder.x, rightShoulder.y-10);
			ofVertex(rightShoulder.x-10, rightShoulder.y-20);
			
			ofVertex(leftShoulder.x+10, leftShoulder.y-20);	
			ofVertex(leftShoulder.x, leftShoulder.y-10);
			ofVertex(leftShoulder.x, leftShoulder.y);	
		
			ofVertex(leftShoulder.x, leftHip.y);
			ofVertex(leftShoulder.x+20, leftHip.y+40);
			//ofVertex(torsoJoint.x, rightHip.y+35);
			ofVertex(rightShoulder.x-20, rightHip.y+40);	
			ofVertex(rightShoulder.x, rightHip.y);
			
			ofEndShape();
		
			//draw upper right leg
			ofBeginShape();
			int upperLegLength =  ofDist(rightHip.x, rightHip.y, rightKnee.x, rightKnee.y);
			int upperLeg_width = upperLegLength/10;
			ofVertex((rightHip.x + upperLeg_width), rightHip.y);
			ofVertex((rightHip.x - upperLeg_width), rightHip.y);
			ofVertex((rightKnee.x - upperLeg_width), rightKnee.y);
			ofVertex((rightKnee.x + upperLeg_width), rightKnee.y);
			ofEndShape();
		
			//draw upper left leg
			ofBeginShape();
			ofVertex((leftHip.x + upperLeg_width), leftHip.y);
			ofVertex((leftHip.x - upperLeg_width), leftHip.y);
			ofVertex((leftKnee.x - upperLeg_width), leftKnee.y);
			ofVertex((leftKnee.x + upperLeg_width), leftKnee.y);
			ofEndShape();
		
			//draw lower right leg
			ofBeginShape();
			int lowerLegLength =  ofDist(rightKnee.x, rightKnee.y, rightFoot.x, rightFoot.y);
			int lowerLeg_width = lowerLegLength/8;
			ofVertex((rightKnee.x + lowerLeg_width), rightKnee.y);
			ofVertex((rightKnee.x - lowerLeg_width), rightKnee.y);
			ofVertex((rightFoot.x - lowerLeg_width), rightFoot.y);
			ofVertex((rightFoot.x + lowerLeg_width), rightFoot.y);
			ofEndShape();
		
			//draw lower left leg
			ofBeginShape();
			ofVertex((leftKnee.x + lowerLeg_width), leftKnee.y);
			ofVertex((leftKnee.x - lowerLeg_width), leftKnee.y);
			ofVertex((leftFoot.x - lowerLeg_width), leftFoot.y);
			ofVertex((leftFoot.x + lowerLeg_width), leftFoot.y);
			ofEndShape();
		
			//user.drawSkeleton(); //only if we're NOT playing, draw the silhouette... maybe you want it like this?
			
		if (vel_left.length() > vel_right.length()) {
			vel = vel_left;
			hand = leftHand;
			prevHand = prev_leftHand;
		} else {
			vel = vel_right;
			hand = rightHand;
			prevHand = prev_rightHand;
		}
		
		if (gameStatus == 1){ //only check the ball, and then draw ball stuff if we're playing!
			
			myBall.update(hand, prevHand, vel);
			
			if ((target.x - 40) < myBall.pos.x && myBall.pos.x < (target.x + 40) &&
				(target.y - 40) < myBall.pos.y && myBall.pos.y < target.y + 40) {
				
				
				hasHit = true;
				now = ofGetElapsedTimef();
				lastTarget.x = target.x;
				lastTarget.y = target.y;
				
				
				//cout << "wtf" << endl;
                
			} 


            
			ofNoFill();
			ofSetColor(ofColor::white);		
			ofSetLineWidth(5);
			ofCircle(target.x, target.y, 50);
			
			ofFill();
			
			drawSplat();
			
			for (int i = 0; i < splatList.size(); i++ ) {
				splatList[i].draw();
				
			}
			
			prev_rightHand = rightHand;
			prev_leftHand = leftHand;
		}
	}
    
//    if (gameStatus == 0){
//        // this is your start screen !
//        
//        ofBackground(10);
//        ofSetColor(255);
//        ofDrawBitmapString("this is the start screen!", 200, 200);
//		// make start button
//        
//    }
	
	if (gameStatus == 1){
		
		
		myBall.draw(hand);	
		
		ofDisableBlendMode();
		ofPopMatrix();
		
		// draw some info regarding frame counts etc
		//	ofSetColor(0, 255, 0);
		//string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
		//    
		//	verdana.drawString(msg, 20, openNIDevice.getNumDevices() * 480 - 20);
		
		//cout << vel.length() << endl;
		
		
		long currTime = ofGetElapsedTimeMillis();
		//int min = 60000;
		
		if (currTime <= 15000) {
			ofSetColor(255);
			string msg = "Food Fight!";
			verdana.drawString(msg, (WIDTH/.99)-150, 100);
		}
		
		
		//ofDrawBitmapString("Current Score: "+ofToString(score), (WIDTH/2), 50);
//		ofDrawBitmapString("Time (seconds): "+ofToString((ofGetElapsedTimeMillis()/1000)), (WIDTH/4), 50);

	//} else if (gameStatus == 2) {
//		
//		//	ofRect(	//draw rect over background
//		
//		//restart button
//		ofSetColor(ofColor::fuchsia);
//		ofCircle(WIDTH/2, HEIGHT/2, 50);
//		// (ballX >= ofGetWidth() || ballX <= 0 )
//		
//		// use code for start button
//		int distFromCircle = ofDist(WIDTH/2, HEIGHT/2, hand.x, hand.y);
//		
//		if (distFromCircle < 50) {
//			gameStatus = 1; // set to playing
//			score = 0; //reset score
//			//reset timer
//		}
//		
//		if ( hand.x >= WIDTH/2 && hand.y <= HEIGHT/2  ) {
//			gameStatus = 1; //set to playing
//            score = 0; //reset the score?
//		}
//		
//		//quit button
//		ofSetColor(ofColor::aquamarine);
//		ofCircle(WIDTH/4, HEIGHT/4, 50);
//		
//		
//		string msg = "Final Score:"+ofToString(score);
//		verdana.drawString(msg, WIDTH/3, 100);	//final score
//		//draw game over screen with final score

	}
}

//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::drawSplat(){
	if (now - ofGetElapsedTimef() > - .5 && now - ofGetElapsedTimef()<0 ) {
		currentSplat = myBall.currentFood;
		//splat[currentSplat].draw((lastTarget.x-45), (lastTarget.y-35), 90, 70); //fix and declare
		
		Splat s;
		s.setup( &splat[currentSplat] );
		s.pos.set( lastTarget.x - 45, lastTarget.y - 35 );
		splatList.push_back( s );
		
		//cout << ofToString( now - ofGetElapsedTimef() )<< endl;
		playSplatSound();
	}
}

//--------------------------------------------------------------
void testApp::playSplatSound(){
	
	if (splatSound.getIsPlaying() == false){
		//synth.setPan(ofMap(x, 0, widthStep, -1, 1, true));  <-- how to move it between L and R channels	
		splatSound.play();
	}
}


