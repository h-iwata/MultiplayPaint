package org.cocos2dx.application;

import android.os.Bundle;

import com.parse.Parse;
import com.parse.ParseAnalytics;
import com.parse.ParseInstallation;
import com.parse.PushService;

public class ApplicationDemo extends android.app.Application {

	@Override
	public void onCreate() {
		super.onCreate();

		Parse.initialize(this, "- Parse Application ID -", "- Parse Client Key -");
		PushService.setDefaultPushCallback(this, ApplicationDemoActivity.class);
		ParseInstallation.getCurrentInstallation().saveInBackground();

		// call to native
		nativeInit();
	}

	private native void nativeInit();

	public void parsePushSubscribeToUserChannel(String userId) {
		for(String ch : PushService.getSubscriptions(this)) {
		    PushService.unsubscribe(this, ch);
		}
		PushService.subscribe(this, "u-" + userId, ApplicationDemoActivity.class);
    }
    static {
        System.loadLibrary("demomemory");
    }     

}
