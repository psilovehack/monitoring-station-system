package com.jlccaires.mss.client;

import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.KeyListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.Window;
import com.extjs.gxt.ui.client.widget.form.TextArea;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.RootPanel;

public class MSS implements EntryPoint {

	private final MSSServiceAsync service = GWT.create(MSSService.class);

	public void onModuleLoad() {

		final TextArea history = new TextArea();
		history.setReadOnly(true);

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
							MessageBox.alert("Vixi", caught.toString(), null);
							command.enable();
						}
					});

				}

			}
		});

		Window w = new Window();
		w.setSize(300, 200);
		w.setResizable(false);
		w.setLayout(new FitLayout());
		w.add(history);
		w.setBottomComponent(command);

		RootPanel.get().add(w);
	}
}
