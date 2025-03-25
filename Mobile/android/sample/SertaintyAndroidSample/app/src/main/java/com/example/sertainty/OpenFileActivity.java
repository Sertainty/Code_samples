package com.example.sertainty;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import org.Sertainty.ByteArray;
import org.Sertainty.Challenge;
import org.Sertainty.File;
import org.Sertainty.Sys;

import java.io.InputStream;
import java.util.Locale;

public class OpenFileActivity extends AppCompatActivity {

    private File file;
    private Button btnSubmit;
    private EditText txtResponse;
    private TextView lblPrompt;
    private TextView lblPromptIndex;
    private TextView lblChallengeCount;
    private TextView lblFileName;
    private int chidx = 0;
    private Challenge challenge;
    private Intent goHomeIntent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_file);

        Log.println(Log.INFO,"OpenFileActivity","OnCreate");
        goHomeIntent = new Intent(this,MainActivity.class);

        file = AppState.getInstance().getOpenFile();
        txtResponse = findViewById(R.id.txtResponse);
        lblPrompt = findViewById(R.id.lblPrompt);
        lblPromptIndex = findViewById(R.id.lblPromptIndex);
        lblChallengeCount = findViewById(R.id.lblChallengeCount);
        lblFileName = findViewById(R.id.lblFileName);


        btnSubmit = findViewById(R.id.btnSubmit);
        btnSubmit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.println(Log.INFO,"OnSubmit","Entering");
                if(file != null && file.isOpen() && !file.isAuthorized()) {
                    Log.println(Log.INFO,"OnSubmit","Outer check pass");
                    if (challenge != null) {
                        Log.println(Log.INFO,"OnSubmit","Challenge is valid");
                        String response = txtResponse.getText().toString();
                        challenge.setValue(response);
                        challenge.endTimer();
                        file.addResponse(challenge);
                        Log.println(Log.INFO,"OnSubmit","Response added to file");
                        txtResponse.setText("");
                        challenge = null;
                        if (chidx < file.getChallengeCount() - 1) {
                            Log.println(Log.INFO,"OnSubmit","More challenges exist");
                            setChidx(chidx+1);
                            loadNextChallenge();
                        } else {
                            Log.println(Log.INFO,"OnSubmit","Ready to authenticate.");
                            processAuthentication();
                        }
                    }
                }
            }
        });

        if(file == null)
        {
            Log.println(Log.INFO,"OpenFileActivity","File not open. Launching selector.");
            Intent openFileIntent = new Intent(Intent.ACTION_GET_CONTENT);
            openFileIntent.setType("*/*");
            startActivityForResult(openFileIntent,0);
        }
        else if( file.isOpen())
        {
            lblFileName.setText(file.fileSpecification());
            if(file.isAuthorized())
            {
                Log.println(Log.INFO,"OpenFileActivity","File already authorized. Going to contents.");
                Intent showContentIntent = new Intent(this,FileContentsActivity.class);
                startActivity(showContentIntent);
                finish();
            }
        }
    }

    private void processNextChallenge()
    {

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);


        if(resultCode != RESULT_OK)
        {
            startActivity(goHomeIntent);
            finish();
        }
        else
        {
            Log.println(Log.INFO,"ActivityResult","Open File");
            Uri fileUri = data.getData();

            ByteArray sandboxFile = new ByteArray();
            if(AppState.getInstance().loadInSandbox(fileUri,sandboxFile)) {
                String fileName = sandboxFile.getString();

                Log.println(Log.INFO, "OpenFile", fileName);

                String message;
                file = File.open(fileName, 2);
                if (file.getCallStatus().hasError()) {
                    message = "Unable to open file: " + file.getCallStatus().getErrorMessage();
                    Log.println(Log.ERROR, "OpenFile", message);
                    AppState.getInstance().releaseOpenFile();
                    startActivity(goHomeIntent);
                } else {
                    lblFileName.setText(file.fileSpecification());
                    AppState.getInstance().setOpenFile(file);
                    processAuthentication();
                }
            }
        }
    }

    public int getChidx() {
        return chidx;
    }

    public void setChidx(int chidx) {
        this.chidx = chidx;
        lblPromptIndex.setText(String.valueOf(chidx+1));
    }

    private void processAuthentication() {
        int status = file.authenticate();

        if(file.isAuthorized())
        {
            Log.println(Log.INFO,"Authentication","Authorized: Switching to File Contents.");
            Intent showContentIntent = new Intent(this, FileContentsActivity.class);
            startActivity(showContentIntent);
            finish();
        }
        else
        {
            if(status != File.StatusChallenged)
            {
                Log.println(Log.ERROR,"Authentication","Not Authorized: Switching to Home.");
                AppState.getInstance().releaseOpenFile();
                startActivity(goHomeIntent);
            }
            else
            {
                lblChallengeCount.setText(String.format(Locale.getDefault(), "/%d",file.getChallengeCount()));
                setChidx(0);
                loadNextChallenge();
            }
        }
    }

    private void loadNextChallenge() {
        Log.println(Log.INFO,"LoadNextChallenge", "Chidx: " + String.valueOf(chidx));

        challenge = file.getChallenge(chidx);
        lblPrompt.setText(challenge.getPrompt());
        challenge.startTimer();
    }
}
