package com.shengxing.socketprocess;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Process;
import android.util.Log;
import java.util.Timer;
import java.util.TimerTask;

public class ProcessService extends Service {
  private final String TAG = ProcessService.class.getName();
  private int i = 0;

  public ProcessService() {
  }

  @Override public void onCreate() {
    super.onCreate();
    Watcher watcher = new Watcher();
    try {

      watcher.createWatcher(String.valueOf(Process.myUid()));
      watcher.connectToMonitor();
    } catch (Exception e) {
      e.printStackTrace();
      watcher = new Watcher();
      watcher.createWatcher(String.valueOf(Process.myUid()));
      watcher.connectToMonitor();
    }
    Timer timer = new Timer();

    timer.schedule(new TimerTask() {
      @Override public void run() {
        Log.i(TAG, "service is running....." + i);
        i++;
      }
    }, 0, 1000 * 3);
  }

  @Override public IBinder onBind(Intent intent) {
    // TODO: Return the communication channel to the service.
    throw new UnsupportedOperationException("Not yet implemented");
  }
}
