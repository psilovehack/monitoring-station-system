package com.jlccaires.mss.client;

import java.util.ArrayList;

import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("mssService")
public interface MSSService extends RemoteService {
	
	String sendCommand(String command);
	
	ArrayList<String> getPorts();
	
	String connect(String portId) throws Exception;

	String close();
}
