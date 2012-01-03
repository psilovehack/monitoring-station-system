package com.jlccaires.mss.client;

import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("mssService")
public interface MSSService extends RemoteService {
	
	public String sendCommand(String command) throws IllegalArgumentException;
}
