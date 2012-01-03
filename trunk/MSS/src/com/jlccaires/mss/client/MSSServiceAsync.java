package com.jlccaires.mss.client;

import com.google.gwt.user.client.rpc.AsyncCallback;

public interface MSSServiceAsync {
	
	public void sendCommand(String command, AsyncCallback<String> callback)
			throws IllegalArgumentException;
}
