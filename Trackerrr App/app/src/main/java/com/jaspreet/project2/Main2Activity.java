package com.jaspreet.project2;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

public class Main2Activity extends AppCompatActivity {

    public String lat,lng,str;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);

        Intent i = getIntent();
        lat = i.getStringExtra("lat");
        lng = i.getStringExtra("lng");
        str = "Latitude : "+lat+"\n"+"Longitude : "+lng;
        Toast.makeText(this,str,Toast.LENGTH_SHORT).show();

        TextView loc = (TextView)findViewById(R.id.textView);
        loc.setText(str);
    }

}
