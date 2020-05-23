package com.example.tarea1_sistemasoperativos;

import androidx.appcompat.app.AppCompatActivity;


import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import java.util.prefs.Preferences;

public class Configuracion extends AppCompatActivity {

    private SharedPreferences preferences;
    private SharedPreferences.Editor editor;
    private EditText txtPort, txtId;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_configuracion);
        //configuramos los textos que se van a utiizar
        txtPort = findViewById(R.id.txtPortId);
        txtId   = findViewById(R.id.txtIpId);

        preferences = getApplicationContext().getSharedPreferences("MyPref",0);
         editor = preferences.edit();
        //Recuperamos los datos de las preferencias
        String port = preferences.getString("Port",null);
        String id   = preferences.getString("Ip",null);

        //colocamos los datos en el editText que muestra en pantalla la configuracion.
        txtPort.setText(port);
        txtId.setText(id);
    }
    /*
    * This method change the values saved on preferences.
     */
    public void PushSave(View view) {
        if(!txtId.getText().toString().isEmpty() & !txtPort.getText().toString().isEmpty()) {
            //mediante editor podemos reestablecer la configuracion de los datos almacenados en preferencias
            editor.putString("Port", txtPort.getText().toString());
            editor.putString("Ip", txtId.getText().toString());
            editor.commit();
            //mostramos al usuario que la configuracion ha sido almacenada
            Toast.makeText(this,"Datos almacenados",Toast.LENGTH_LONG).show();

            //volvemos a la pantalla de carga de la imagen
            Intent intent = new Intent(this.getApplicationContext(),MainActivity.class);
            startActivityForResult(intent,0);

        }else{
            Toast.makeText(this,"data is not competed",Toast.LENGTH_LONG).show();
        }

    }
}
