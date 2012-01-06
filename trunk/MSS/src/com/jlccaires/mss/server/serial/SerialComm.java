package com.jlccaires.mss.server.serial;


import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;

public class SerialComm implements SerialPortEventListener {

	private SerialPort serialPort;
	/** Buffered input stream from the port */
	private InputStream input;
	/** The output stream to the port */
	private OutputStream output;
	/** Milliseconds to block while waiting for port open */
	private static final int TIME_OUT = 2000;
	/** Default bits per second for COM port. */
	private static final int DATA_RATE = 9600;

	/** Array that stores the added listeners*/
	private ArrayList<DataReceivedListener> dataListeners = new ArrayList<DataReceivedListener>();

	/**Map with the listed ports*/
	private HashMap<String, CommPortIdentifier> portsMap = new HashMap<String, CommPortIdentifier>(); 

	private boolean disableListener = false;

	public SerialComm() {
	}

	public ArrayList<String> listPorts() {
		
		portsMap.clear();

		Enumeration<?> portEnum = CommPortIdentifier.getPortIdentifiers();
		
		 ArrayList<String> portIdList = new ArrayList<String>();

		// iterate through, looking for the port
		while (portEnum.hasMoreElements()) {

			CommPortIdentifier currPort = (CommPortIdentifier) portEnum.nextElement();

			if(currPort.getPortType() == CommPortIdentifier.PORT_SERIAL) {
				portsMap.put(currPort.getName(), currPort);
				portIdList.add(currPort.getName());
			}
		}
		return portIdList;

	}

	public String connect(String portId) {
		
		CommPortIdentifier port = portsMap.get(portId);

		if (port == null) {
			return "Could not find COM port.";
		}

		try {
			// open serial port, and use class name for the appName.
			serialPort = (SerialPort) port.open(this.getClass().getName(), TIME_OUT);

			// set port parameters
			serialPort.setSerialPortParams(DATA_RATE,
					SerialPort.DATABITS_8,
					SerialPort.STOPBITS_1,
					SerialPort.PARITY_NONE);

			// open the streams
			input = serialPort.getInputStream();
			output = serialPort.getOutputStream();

			// add event listeners
			serialPort.addEventListener(this);
			serialPort.notifyOnDataAvailable(true);
			
			return "Connected.";

		} catch (Exception e) {
			e.printStackTrace();
			return e.toString();
		}

	}

	/**
	 * This should be called when you stop using the port.
	 * This will prevent port locking on platforms like Linux.
	 */
	public synchronized void close() {
		if (serialPort != null) {
			serialPort.removeEventListener();
			serialPort.close();
		}
	}

	public String print(String action) {

		try {
			//disable listener on print to return the value
			disableListener = true;
			output.write(action.getBytes());

			Thread.sleep(100);

			return getSerialData();

		} catch (Exception e) {
			e.printStackTrace();
			return e.toString();
		}

	}

	/**
	 * Handle an event on the serial port.
	 */
	public synchronized void serialEvent(SerialPortEvent oEvent) {

		if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {

			if(!disableListener){

				for (DataReceivedListener listener : dataListeners)
					listener.dataReceived(getSerialData());

			} else {
				disableListener = false;
			}

		}

	}

	private String getSerialData() {

		try {

			String data = "";
			while (input.available() > 0) {

				Thread.sleep(10);
				data += (char)input.read();
			}

			return data;

		} catch (Exception e) {
			e.printStackTrace();
			return e.toString();
		}
		

	}

	public void addDataReceivedListener(DataReceivedListener listener){
		dataListeners.add(listener);
	}

}