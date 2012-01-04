package com.jlccaires.mss.client;

import java.util.ArrayList;

import com.google.gwt.user.client.rpc.AsyncCallback;

public interface MSSServiceAsync {
	
	void sendCommand(String command, AsyncCallback<String> callback);

	void getPorts(AsyncCallback<ArrayList<String>> callback);

	void connect(String portId, AsyncCallback<String> callback);
}
