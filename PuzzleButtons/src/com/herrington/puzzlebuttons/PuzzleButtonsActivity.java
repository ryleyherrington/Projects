package com.herrington.puzzlebuttons;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;


public class PuzzleButtonsActivity extends Activity {
	private Button btnStart;
	private Button btnDir;
	private Spinner numspinner; 
	private Spinner themespinner;
	protected AppPreferences appPrefs;
	protected AppPreferences appPrefs2;
	private TextView tv;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		//Set activity layout
		setContentView(R.layout.main);
		
		
		numspinner = (Spinner)findViewById(R.id.spinnernum);
		ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this, R.array.size_array, android.R.layout.simple_spinner_item);
		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		numspinner.setAdapter(adapter);

		numspinner.setOnItemSelectedListener(new OnItemSelectedListener(){
			public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
				int buttonNum = Integer.parseInt(numspinner.getSelectedItem().toString());
				appPrefs = new AppPreferences(getApplicationContext());
				appPrefs.saveNum(buttonNum);
			}
			public void onNothingSelected(AdapterView<?> arg0) {
				// TODO Auto-generated method stub
			}
		});
		
		themespinner = (Spinner)findViewById(R.id.spinnertheme);
		ArrayAdapter<CharSequence> adapter2 = ArrayAdapter.createFromResource(this, R.array.theme_array, android.R.layout.simple_spinner_item);
		adapter2.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		themespinner.setAdapter(adapter2);

		themespinner.setOnItemSelectedListener(new OnItemSelectedListener(){
			public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
				String themeCs = themespinner.getSelectedItem().toString();
				appPrefs2 = new AppPreferences(getApplicationContext());
				appPrefs2.saveThemeColors(themeCs);
			}
			public void onNothingSelected(AdapterView<?> arg0) {
				// TODO Auto-generated method stub
			}
		});


		//Get button reference
		btnStart = (Button)findViewById(R.id.dir_Button);

		//Set Click Listener
		btnStart.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				//Start next activity
				Intent myIntent = new Intent(PuzzleButtonsActivity.this, gameActivity.class);
				PuzzleButtonsActivity.this.startActivity(myIntent);
			
				
			
			}
		});

	}
}

