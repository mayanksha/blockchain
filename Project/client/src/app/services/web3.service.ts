import { InjectionToken } from '@angular/core';
import Web3 from 'web3';

export const WEB3 = new InjectionToken<Web3>('web3', {
  providedIn: 'root',
  factory: () => {
    try {
      /*const provider = ('ethereum' in window) ? window['ethereum'] : Web3.givenProvider;*/
      const provider = new Web3(Web3.givenProvider || "http://localhost:8545");
      /*return new Web3(new Web3.providers.HttpProvider("http://localhost:8545"));*/
      return provider;
    } catch (err) {
      throw new Error('Non-Ethereum browser detected. You should consider trying Mist or MetaMask!');
    }
  }
});
