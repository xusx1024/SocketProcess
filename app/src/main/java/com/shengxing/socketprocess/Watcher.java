package com.shengxing.socketprocess;

/**
 * Fun:
 * Created by sxx.xu on 6/20/2018.
 */

public class Watcher {

  static {
    System.loadLibrary("native-lib");
  }

  public native void createWatcher(String userId);

  public native void connectToMonitor();
}
