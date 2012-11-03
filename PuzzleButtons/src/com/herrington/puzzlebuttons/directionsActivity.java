package com.herrington.puzzlebuttons;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;


public class directionsActivity extends Activity{
	public Button btnB;
	
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//Set activity layout
		setContentView(R.layout.dir);
		btnB = (Button)findViewById(R.id.btnBack);

		//Set Click Listener
		btnB.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				//Start dir activity
				Intent myIntent = new Intent(directionsActivity.this, PuzzleButtonsActivity.class);
				directionsActivity.this.startActivity(myIntent);
			}
		});
	}
}