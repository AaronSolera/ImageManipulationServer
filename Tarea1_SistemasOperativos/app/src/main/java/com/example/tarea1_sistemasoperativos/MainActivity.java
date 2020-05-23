package com.example.tarea1_sistemasoperativos;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Entity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.os.StrictMode;
import android.provider.MediaStore;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.net.Uri;
import android.widget.ListView;
import android.widget.Spinner;


import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import java.io.BufferedReader;
import java.io.DataOutput;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import java.io.File;

import javax.net.ssl.HttpsURLConnection;


public class MainActivity extends AppCompatActivity {

    private ImageView imagen;
    private Bitmap bitmap;
    private Button btnSend;
    private String StringPixeles;
    private String imgname;
    private SharedPreferences preferences;
    private Spinner opciones;
    private String metodOption = "0";
    StringBuffer stringBuffer = new StringBuffer();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (android.os.Build.VERSION.SDK_INT > 9)
        {
            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
            StrictMode.setThreadPolicy(policy);
        }
        preferences = getApplicationContext().getSharedPreferences("MyPref",0);
        //cargamos elementos de la interfaz de usuario
        btnSend = findViewById(R.id.btnSend);
        imagen = findViewById(R.id.imagenID);
        opciones = findViewById(R.id.spinner);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,R.array.select,android.R.layout.simple_spinner_item);
        opciones.setAdapter(adapter);

        opciones.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                if(position == 0){
                    metodOption = "0";
                }
                if(position == 1){
                    metodOption = "1";
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }

    public void onClick(View view) {
        CargarImagen();
    }
    private void CargarImagen(){
        Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        intent.setType("image/");
        startActivityForResult(intent.createChooser(intent,"seleccione la imagen"),10);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(resultCode == RESULT_OK){
            Uri path = data.getData();
            imagen.setImageURI(path);

            //calculamos los rgb de la imagen para cada pixel
            getRGB();
            //habilitamos el boton para subir la imagen
            btnSend.setVisibility(View.VISIBLE);
            btnSend.setEnabled(true);

            File file = new File(String.valueOf(path));
            imgname = file.getName();
            //imgname = path.getLastPathSegment();
            //imgname = (String) imagen.getTag();
            System.out.println(imgname+ " OPA");
            System.out.println(imagen.getTag() + " tag");

        }
    }
    private void getRGB(){
        BitmapDrawable bitmapDrawable = (BitmapDrawable) imagen.getDrawable();
        bitmap = bitmapDrawable.getBitmap();
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        int r,g,b;
        //StringPixeles = "";
        //byte[] bytePixeles = new byte[3*width*height];

        //bucle para obtener todos los pixeles
        int positionArray = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int pixel = bitmap.getPixel(j,i);
                r = Color.red(pixel);
                g = Color.green(pixel);
                b = Color.blue(pixel);

                stringBuffer = stringBuffer.append(r);
                stringBuffer = stringBuffer.append(",");
                stringBuffer = stringBuffer.append(g);
                stringBuffer = stringBuffer.append(",");
                stringBuffer = stringBuffer.append(b);
                stringBuffer = stringBuffer.append(",");
            }
        }
    }

    public void settings(View view) {
        Intent intent = new Intent(view.getContext(),Configuracion.class);
        startActivityForResult(intent,0);
    }

    public void SendIMG(View view) {
        BitmapDrawable bitmapDrawable = (BitmapDrawable) imagen.getDrawable();
        bitmap = bitmapDrawable.getBitmap();


       // JSONObject jsonObject = new JSONObject();
       // try {
        StringBuffer txtToSend = new StringBuffer();
        txtToSend.append("name: "+imgname+"\n");
        txtToSend.append("width: "+String.valueOf(bitmap.getWidth())+"\n");
        txtToSend.append("height: "+String.valueOf(bitmap.getHeight())+"\n");
        txtToSend.append("option: "+metodOption+"\n");
        txtToSend.append("img: ");
        stringBuffer.insert(0, txtToSend);
        stringBuffer.append('\n');

        //txtToSend.append("img: "+stringBuffer+"\n");
        String ip   = preferences.getString("Ip",null);

        //String urlsdfa = "http://" + ip +"/";

        try{
            String vaca = "name: imagen\n width: 40\n height: 25\n option: 1\n img: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,0,0,0,212,212,212,255,255,255,255,255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,0,0,0,255,255,255,255,255,255,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,239,239,239,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,239,239,239,0,0,0,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,0,0,0,0,0,0,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,0,0,0,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,0,0,0,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,0,0,0,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,174,201,255,174,201,255,174,201,255,255,255,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,255,255,255,174,201,255,174,201,255,174,201,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,174,201,255,174,201,255,174,201,255,255,255,255,255,255,255,174,201,255,174,201,255,174,201,255,174,201,255,255,255,255,255,255,255,174,201,255,174,201,255,174,201,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,174,201,255,174,201,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,174,201,255,174,201,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";

            URL url = new URL("http://34.75.94.250:1717");
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("POST");
            connection.setRequestProperty("USER-AGENT","mobileAPP");
            //connection.setRequestProperty("CONTENT-LENGTH",String.valueOf(vaca.length()));
            connection.setDoOutput(true);
            DataOutputStream dStream = new DataOutputStream(connection.getOutputStream());
            //dStream.writeBytes(String.valueOf(txtToSend));
            // vaca = "caiga";
            dStream.writeBytes(vaca);
            dStream.flush();
            dStream.close();
            System.out.println("Exito");
        }catch (Exception e){
            System.out.println("Solo no funionó");
            e.printStackTrace();
        }/*
        try {
            String vaca = "name: imagen\n width: 40\n height: 25\n option: 1\n img: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,0,0,0,212,212,212,255,255,255,255,255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,0,0,0,255,255,255,255,255,255,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,239,239,239,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,53,53,53,53,53,53,53,53,53,53,53,53,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,239,239,239,0,0,0,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,0,0,0,0,0,0,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,0,0,0,239,239,239,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,0,0,0,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,0,0,0,212,212,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,174,201,255,174,201,255,174,201,255,255,255,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,255,255,255,174,201,255,174,201,255,174,201,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,174,201,255,174,201,255,174,201,255,255,255,255,255,255,255,174,201,255,174,201,255,174,201,255,174,201,255,255,255,255,255,255,255,174,201,255,174,201,255,174,201,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,255,174,201,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,239,239,239,255,255,255,255,255,255,255,255,255,255,174,201,255,174,201,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,174,201,255,174,201,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,255,255,255,255,174,201,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,212,212,212,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n";
            HttpClient client = new DefaultHttpClient();
            HttpPost post = new HttpPost("http://34.75.94.250:1717");
            post.addHeader("User-Agent","mobileAPP");
            post.addHeader("Content-Type", "text/plain");
            HttpEntity entity = new ByteArrayEntity(vaca.getBytes("UTF-8"));
            //String entityy = new StringEntity(,ContentType)
            post.setEntity(entity);
            HttpResponse response = client.execute(post);
            String result = EntityUtils.toString(response.getEntity());
            System.out.println(result);

        }catch (Exception e){
            System.out.println("Fail");
            e.printStackTrace();
        }
*/

          //  jsonObject.put("width", String.valueOf(bitmap.getWidth()));
          //  jsonObject.put("height",String.valueOf(bitmap.getHeight()));

          //  jsonObject.put("img",stringBuffer);
          //  System.out.println(String.valueOf(jsonObject));
          //  Toast.makeText(this,StringPixeles,Toast.LENGTH_LONG).show();
       // }catch (JSONException e){
        //    e.printStackTrace();
       /// }
        //Toast.makeText(this,"Falta que habiliten el server",Toast.LENGTH_LONG).show();
    }
}
