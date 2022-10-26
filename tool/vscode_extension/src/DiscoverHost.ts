import * as fs from 'fs';
import * as net from 'net';
import * as vscode from 'vscode';

const PipeName = '\\\\.\\pipe\\Zplutor.RunAnywhere.Discover.VSCode';

export class DiscoverHost {

    public constructor() {

        this.server = net.createServer(socket => {

            let activePath = this.getActivePath();

            console.log('Connect from client. Active path: ' + activePath);
    
            let responseBufffer = Buffer.from(activePath, 'utf8');
    
            let lengthBuffer = Buffer.alloc(4);
            lengthBuffer.writeUint32LE(responseBufffer.length);
            
            socket.write(lengthBuffer);
            socket.write(responseBufffer);
            socket.end();

            socket.on('close', hasError => {
                console.log('Client socket closed.');
            });
        });

        this.server.on('error', error => {

            console.log('Server error. ' + error.message);

            if (this.canErrorRetry(error)) {

                setTimeout(() => {
                    
                    console.log('Retry to start server.');

                    if (this.isStarted) {
                        this.startServer();
                    }
                }, 100);
            }            
        });
    }

    private canErrorRetry(error: Error & { code?: string }): boolean {
        return error.code == 'EADDRINUSE';
    }

    public start() {

        this.isStarted = true;
        this.startServer();
    }

    private startServer() {

        if (!this.server.listening) {
            this.server.listen(PipeName);
        }
    }

    public stop() {

        this.isStarted = false;

        if (this.server.listening) {
            this.server.close();
        }
    }

    private getActivePath(): string {

        let fileUri = vscode.window.activeTextEditor?.document.uri;
        if (!fileUri) {
            return '';
        }

        if (fileUri.scheme != 'file') {
            return '';
        }

        let result = fileUri.fsPath;

        let workspaceFolders = vscode.workspace.workspaceFolders;
        if (workspaceFolders) {

            for (let folder of workspaceFolders) {

                if (fileUri.fsPath.toLowerCase().startsWith(folder.uri.fsPath.toLowerCase())) {
                    result += '|' + folder.uri.fsPath;
                    break;
                }
            }
        }

        return result;
    }

    private server: net.Server;
    private isStarted = false;
}