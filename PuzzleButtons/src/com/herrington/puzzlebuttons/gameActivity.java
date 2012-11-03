package com.herrington.puzzlebuttons;

import android.app.Activity;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.graphics.Color;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.Toast;
import android.widget.ToggleButton;


public class gameActivity extends Activity implements View.OnClickListener, OnSharedPreferenceChangeListener {
	int size;
	String theme; 
	int n= 4;
	int w = 0;
	int h = 0;
	Boolean bc[] = new Boolean[n*n];
	DisplayMetrics metrics = new DisplayMetrics();
	protected AppPreferences appPrefs;
	protected AppPreferences appPrefs2;
	int offTog;
	int onTog;


	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.gamen);

		appPrefs = new AppPreferences(getApplicationContext());
		size = appPrefs.getNumber();

		appPrefs2 = new AppPreferences(getApplicationContext());
		theme=appPrefs2.getThemeColors();
		//Toast.makeText( this, theme, Toast.LENGTH_SHORT ).show();

		if (theme.equalsIgnoreCase("Blazers")){
			offTog = Color.BLACK;
			onTog = Color.RED;
		}
		if (theme.equalsIgnoreCase("Ducks")){
			offTog = Color.YELLOW;
			onTog = Color.GREEN;
		}
		if (theme.equalsIgnoreCase("Beavers")){
			offTog = Color.BLACK;
			onTog = Color.rgb(255,140,0);
		}
		if (theme.equalsIgnoreCase("Girl")){
			offTog = Color.rgb(255,20,147);
			onTog = Color.rgb(148,0, 211);
		}
		if (theme.equalsIgnoreCase("Boy")){
			offTog = Color.BLUE;
			onTog = Color.rgb(39,139,34);
		}

		n = size;
		getWindowManager().getDefaultDisplay().getMetrics(metrics);

		w = ((metrics.widthPixels)/n);
		h = (metrics.heightPixels)/(n+4);	

		TableLayout layout = new TableLayout (this);
		layout.setLayoutParams( new TableLayout.LayoutParams(4,5) );
		layout.setPadding(1,1,1,1);
		layout.setBackgroundDrawable(getResources().getDrawable(R.drawable.background2));

		for (int r=0; r<n; r++) {
			TableRow tr = new TableRow(this);
			tr.setPadding(10,(h/n)-10,1,(h/n)-10);

			for(int c=0; c<n; c++) {
				ToggleButton  tb = new ToggleButton (this); 
				tb.setId(r*n+c);
				bc[tb.getId()] = false;
				tb.setText("");
				tb.setBackgroundColor(offTog);
				tb.setOnClickListener(this);
				tr.addView(tb,w-10,h);
			}

			layout.addView(tr);

		}
		super.setContentView(layout);
	}

	private boolean isLeftEdge(int i)  { return ((i%n) == 0);   }
	private boolean isRightEdge(int i) { return ((i%n) == n-1); }
	private boolean isUpperEdge(int i) { return (i<n);          }
	private boolean isLowerEdge(int i) { return (i>=(n*(n-1))); }

	private void tog(int id){
		assert id>0;
		assert id<n*n;
		ToggleButton tbtn = (ToggleButton) findViewById(id);
		bc[id] = !bc[id];
		tbtn.setChecked(bc[id]);
		tbtn.setText("");
		if(bc[id]){
			tbtn.setBackgroundColor(onTog);
		}
		else tbtn.setBackgroundColor(offTog);

	}

	private void changeState(ToggleButton tbtn) {
		int index = tbtn.getId();

		if (!isLeftEdge(index)) {
			tog(index-1);
		}
		if (!isRightEdge(index)){
			tog(index+1);
		}
		if (!isUpperEdge(index)){
			tog(index-n);
		}
		if (!isLowerEdge(index)){
			tog(index+n);
		}

	}
	public boolean checkWin(){
		for (int i = 0; i<n*n; i++){
			if (!bc[i]){
				return false;
			}
		}
		Toast.makeText( this, "You've won... this round.", Toast.LENGTH_SHORT ).show();
		return true;
	}	
	public void onClick(View v) {
		int id = v.getId();
		tog(id);
		changeState((ToggleButton) v);
		checkWin();
	}

	public void onSharedPreferenceChanged(SharedPreferences prefs, String key) {
		if (key.equals("PREF_NUM")) {
			size = (prefs.getInt("sharednum", 3));
		}
		if (key.equals("PREF_THEME")){
			theme = prefs.getString("sharedtheme", "blazers");
		}
		// etc
	}
}


