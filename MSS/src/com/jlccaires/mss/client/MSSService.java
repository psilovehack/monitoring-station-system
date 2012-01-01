package com.jlccaires.mss.client;

import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("mssService")
public interface MSSService extends RemoteService {
	String greetServer(String name) throws IllegalArgumentException;
}
