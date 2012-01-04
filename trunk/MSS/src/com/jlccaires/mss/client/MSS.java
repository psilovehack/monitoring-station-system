package com.jlccaires.mss.client;

import java.util.ArrayList;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.KeyListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.Window;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.TextArea;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.form.ComboBox.TriggerAction;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.toolbar.FillToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.RootPanel;

public class MSS implements EntryPoint {

	private final MSSServiceAsync service = GWT.create(MSSService.class);

	public void onModuleLoad() {
		
		final SimpleComboBox<String> comboPorts = new SimpleComboBox<String>();
		comboPorts.setTriggerAction(TriggerAction.ALL);
		comboPorts.setEditable(false);
		comboPorts.setFieldLabel("COM Ports");
		
		service.getPorts(new AsyncCallback<ArrayList<String>>() {
			
			public void onSuccess(ArrayList<String> result) {
				comboPorts.add(result);
			}
			
			public void onFailure(Throwable caught) {
				failure(caught);
			}
		});
		
		final TextArea history = new TextArea();
		history.setReadOnly(true);
		
		Button btnConnect = new Button("Connect");
		btnConnect.addSelectionListener(new SelectionListener<ButtonEvent>() {
			
			public void componentSelected(ButtonEvent ce) {
				
				service.connect(comboPorts.getValue().getValue(), new AsyncCallback<String>() {
					public void onSuccess(String result) {
						history.setValue((history.getValue() == null ?
								"" : history.getValue()) + result + "\n");
					}
					public void onFailure(Throwable caught) {
						failure(caught);
					}
				} );
				
			}
		});

		final TextField<String> command = new TextField<String>();
		command.setLabelSeparator("");
		command.addKeyListener(new KeyListener() {
			public void componentKeyDown(ComponentEvent event) {

				if (event.getKeyCode() == 13 || event.getKeyCode() == 52) {
					
					command.disable();

					service.sendCommand(command.getValue(), new AsyncCallback<String>() {

						public void onSuccess(String result) {
							history.setValue((history.getValue() == null ?
									"" : history.getValue()) + result + "\n");
							command.enable();
							command.clear();
						}

						public void onFailure(Throwable caught) {
							failure(caught);
							command.enable();
						}
					});

				}
			}
		});
		
		ToolBar tbSerial = new ToolBar();
		tbSerial.setSpacing(20);
		tbSerial.add(comboPorts);
		tbSerial.add(new FillToolItem());
		tbSerial.add(btnConnect);

		Window w = new Window();
		w.setSize(300, 200);
		w.setResizable(false);
		w.setLayout(new FitLayout());
		w.setTopComponent(tbSerial);
		w.add(history);
		w.setBottomComponent(command);

		RootPanel.get().add(w);
	}
	
	private void failure(Throwable caught) {
		MessageBox.alert("Error", caught.toString(), null);
	}

}
