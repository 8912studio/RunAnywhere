import * as vscode from 'vscode';
import { DiscoverHost } from './DiscoverHost';

let host = new DiscoverHost;

export function activate(context: vscode.ExtensionContext) {

	console.log('Extension is activated.');

	if (vscode.window.state.focused) {
		console.log('Window is focused, start host.');
		host.start();
	}

	context.subscriptions.push(vscode.window.onDidChangeWindowState(state => {

		console.log('Window focus state changed: ' + state.focused);

		if (state.focused) {
			host.start();
		}
		else {
			host.stop();
		}
	}));
}

export function deactivate() {

	host.stop();
}
