package com.herrington.puzzlebuttons;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

public class AppPreferences extends Activity{
     private static final String APP_SHARED_PREFS = "com.herrington.pref"; //  Name of the file -.xml
     
     private SharedPreferences appSharedPrefs;
     private Editor prefsEditor;

     public AppPreferences(Context context){
         this.appSharedPrefs = context.getSharedPreferences(APP_SHARED_PREFS, Activity.MODE_PRIVATE);
         this.prefsEditor = appSharedPrefs.edit();
     }

     public int getNumber() {
         return appSharedPrefs.getInt("buttonNum", 3);
     }
     public void saveNum(int num) {
         prefsEditor.putInt("buttonNum", num);
         prefsEditor.commit();
     }
     
     public String getThemeColors() {
    	 return appSharedPrefs.getString("themeC", "ryley");
     }
     
     public void saveThemeColors(String themeColors){
    	 prefsEditor.putString("themeC", themeColors);
    	 prefsEditor.commit();
     }
     
     
}