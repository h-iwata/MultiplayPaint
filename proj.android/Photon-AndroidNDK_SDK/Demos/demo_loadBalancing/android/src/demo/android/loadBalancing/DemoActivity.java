package demo.android.loadBalancing;

import java.util.Timer;
import java.util.TimerTask;

import demo.android.loadBalancing.R;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ScrollView;
import android.widget.TextView;

public class DemoActivity extends Activity
{
	private final int STATE_INITIALIZED = 0;
	private final int STATE_CONNECTING = 1;
	private final int STATE_CONNECTED = 2;
	private final int STATE_JOINING = 3;
	private final int STATE_JOINED = 4;
	private final int STATE_LEAVING = 5;
	private final int STATE_LEFT = 6;
	private final int STATE_DISCONNECTING = 7;
	private final int STATE_DISCONNECTED = 8;
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		mTextView = (TextView)findViewById(R.id.txtView);
		mScrollView = (ScrollView)findViewById(R.id.scrollView);
		mViewMasterServer = (FrameLayout)findViewById(R.id.viewMasterServer);
		mViewGameRoom = (FrameLayout)findViewById(R.id.viewGameRoom);

		((Button)findViewById(R.id.buttonLeave)).setOnClickListener(new OnClickListener()
		{
			public void onClick(View v)
			{
				mViewGameRoom.setVisibility(View.INVISIBLE);
				leaveGame();
			}
		});

		((Button)findViewById(R.id.buttonJoin)).setOnClickListener(new OnClickListener()
		{
			public void onClick(View v)
			{
				mViewMasterServer.setVisibility(View.INVISIBLE);
				joinRandomGame();
			}
		});

		((Button)findViewById(R.id.buttonCreate)).setOnClickListener(new OnClickListener()
		{
			public void onClick(View v)
			{
				mViewMasterServer.setVisibility(View.INVISIBLE);
				createGame();
			}
		});

		mHandler = new Handler()
		{
			public void handleMessage(final Message msg)
			{
				switch(msg.what)
				{
				case 0:
					mTextView.append((String) msg.obj);
					mScrollView.scrollTo(0, mTextView.getHeight());
					break;
				case 1:
					gameRun();
					break;
				}
			}
		};
		
		startDemo();

		//start timer for demo
		final int timerPeriod = 100;
		new Timer().scheduleAtFixedRate(new TimerTask() 
		{
			public void run()
			{
				mHandler.sendMessage(mHandler.obtainMessage(1));
			}
		}, timerPeriod, timerPeriod);
	}

	public native void leaveGame();
	public native void createGame();
	public native void joinRandomGame();
	public native void gameRun();
	public native void startDemo();

	static { System.loadLibrary("demoLoadBalancing"); }

	public void addText(String text)
	{
		mHandler.sendMessage(mHandler.obtainMessage(0, text));
	}

	public void stateUpdate(int newState)
	{
		if(newState == STATE_JOINED)
			mViewGameRoom.setVisibility(View.VISIBLE);
		else if(newState == STATE_CONNECTED)
			mViewMasterServer.setVisibility(View.VISIBLE);
	}

	private Handler mHandler;
	private TextView mTextView;
	private ScrollView mScrollView;
	private FrameLayout mViewMasterServer;
	private FrameLayout mViewGameRoom;
}