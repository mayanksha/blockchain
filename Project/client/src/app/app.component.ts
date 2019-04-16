import { Component, Inject, OnInit } from '@angular/core';
import { WEB3 } from './services/web3.service';
import Web3 from 'web3';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  constructor(@Inject(WEB3) private web3: Web3) {  };
  title = 'Blockchain based Voting System';

  async ngOnInit() {
    if ('enable' in this.web3.currentProvider) {
      await this.web3.currentProvider;
    }
  }
}
