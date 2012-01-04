package com.jlccaires.mss.client;

import java.util.ArrayList;

import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("mssService")
public interface MSSService extends RemoteService {
	
	public String sendCommand(String command);
	
	public ArrayList<String> getPorts();
	
	public void connect(String portId);
}
