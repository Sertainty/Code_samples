package com.example.sertainty;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;
import androidx.core.content.FileProvider;
import androidx.recyclerview.widget.RecyclerView;

import org.Sertainty.ByteArray;
import org.Sertainty.Sys;
import org.Sertainty.VirtualFile;
import org.Sertainty.VirtualFileInfo;
import org.Sertainty.VirtualFileInfoList;

import java.io.File;

public class VirtualFileAdapter extends
        RecyclerView.Adapter<VirtualFileAdapter.ViewHolder> {

    private VirtualFileInfoList m_vfInfoList;
    private Context context;

    Intent openFolder;
    Intent openFile = new Intent(Intent.ACTION_VIEW);
    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        context = parent.getContext();
        openFolder = new Intent(context,FileContentsActivity.class);
        LayoutInflater inflater = LayoutInflater.from(context);

        View virtualFileItem = inflater.inflate(R.layout.virtual_file_item,parent,false);
        ViewHolder viewHolder = new ViewHolder(virtualFileItem);
        Log.println(Log.INFO,"Adapter","ViewHolder Created");
        return viewHolder;
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        Log.println(Log.INFO,"View holder","binding");
        final VirtualFileInfo vfInfo = m_vfInfoList.get(position);

        Log.println(Log.INFO,"VirtualFileInfo","Loaded");

        if (vfInfo.typeName().compareTo("Reality") != 0 && vfInfo.typeName().compareTo("Folder") != 0) {
            holder.imgType.setImageDrawable(ContextCompat.getDrawable(context, R.drawable.ic_insert_drive_file_black_24dp));

            holder.txtName.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    ByteArray outbuf = AppState.getInstance().getOpenFile().readAllVirtualFile(vfInfo.fullName());
                    ByteArray extFilename = new ByteArray();
                    if(AppState.getInstance().writeToExternal(vfInfo.name(),outbuf,extFilename)) {
                        Toast.makeText(context, "File written", Toast.LENGTH_SHORT).show();

                        final Uri data = FileProvider.getUriForFile(context, "com.example.sertainty.FileProvider", new File(extFilename.getString()));
                        context.grantUriPermission(context.getPackageName(), data, Intent.FLAG_GRANT_READ_URI_PERMISSION);
                        openFile.setDataAndType(data, "*/*").setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);

                        context.startActivity(openFile);

                    }
                }
            });
        }
        else
        {
            holder.txtName.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Log.println(Log.INFO,"TXTName Click","Clicked");
                    openFolder.putExtra(FileContentsActivity.extraName,vfInfo.fullName());
                    context.startActivity(openFolder);
                }
            });
            holder.imgType.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Log.println(Log.INFO,"IMG Type Click","Clicked");
                    openFolder.putExtra(FileContentsActivity.extraName,vfInfo.fullName());
                    context.startActivity(openFolder);
                }
            });
        }

        holder.txtName.setText(vfInfo.name());

        Log.println(Log.INFO,"View holder","bound");
    }

    @Override
    public int getItemCount() {
        Log.println(Log.INFO,"View holder","Count requested");
        return m_vfInfoList.count();
    }


    public class ViewHolder extends RecyclerView.ViewHolder{

        ImageView imgType;
        TextView txtName;

        public ViewHolder(View itemView)
        {
            super(itemView);
            imgType = itemView.findViewById(R.id.imgType);
            txtName = itemView.findViewById(R.id.txtName);
            Log.println(Log.INFO,"View holder","Initialized");
        }

    }

    public VirtualFileAdapter(VirtualFileInfoList vfInfoList)
    {
        m_vfInfoList = vfInfoList;
        Log.println(Log.INFO,"Adapter","Initialized");
        Log.println(Log.INFO,"Adapter VfInfoList Count", String.valueOf(m_vfInfoList.count()));
    }
}
