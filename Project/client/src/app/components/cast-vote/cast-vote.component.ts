export const ADDRESS = '0x7DbBFDC8cB7D86Ed86ffA8B807Bd7160b82EF8Eb';
import { Component, OnInit, Inject } from '@angular/core';
import { HttpClient, HttpHeaders, HttpErrorResponse } from '@angular/common/http';
import { WEB3 } from '../../services/web3.service';
import Web3 from 'web3';

import { FormArray,
  FormGroup,
  FormBuilder,
  Validators,
  ValidatorFn,
  FormControl,
  NgForm,
  FormGroupDirective
} from '@angular/forms';


@Component({
  selector: 'app-cast-vote',
  templateUrl: './cast-vote.component.html',
  styleUrls: ['./cast-vote.component.css']
})
export class CastVoteComponent implements OnInit {
  public voteCasted = false;
  public form: FormGroup;
  selectedAddr = "";
  public accounts;
  public head;
  public v; public c; public outsider;
  public Manager;
  public Candidate;
  public allcand;

  private ManagerABI;
  private CandidateABI;
  backend = 'http://localhost:8000/';
  constructor(
    private fb: FormBuilder,
    private http: HttpClient,
    @Inject(WEB3) private web3: Web3
  ) { }

  async ngOnInit() {

    this.form = this.fb.group({
      addr: ['', Validators.required ]
    })

    this.accounts = await this.web3.eth.getAccounts();
    this.ManagerABI = await this.http.get(this.backend + 'abi/Manager.json').toPromise();
    this.CandidateABI = await this.http.get(this.backend + 'abi/Candidate.json').toPromise();

    this.Manager = new this.web3.eth.Contract(( this.ManagerABI  as any ), ADDRESS);
    /*this.Candidate = new this.web3.eth.Contract(( this.CandidateABI  as any ), ADDRESS);*/
    console.log( this.Manager.address );

    this.head = this.accounts[0];
    this.outsider = {
      first: this.accounts[7],
      second: this.accounts[8],
      third: this.accounts[9]
    };
    this.getAllCandidates();
  }

  async getAllDetails(candidate) {
    let p1 = candidate.methods.getName().call({from : this.head}); 
    let p2 = candidate.methods.getRollNo().call({from : this.head}); 
    let p3 = candidate.methods.getManifesto().call({from : this.head}); 
    let p = [p1, p2, p3];
    return Promise.all(p)
      .then((arr) => {
        let retObj = {
          Name: arr[0],
          RollNo: arr[1],
          Manifesto: arr[2],
          Address: candidate.address
        }
        return retObj;
      })
      .catch(err => err);
  }

  async getAllCandidates() {
    let det = [];
    let val: any[] = await this.Manager.methods.getAllCandidates().call({from : this.head});
    console.log(val);
    det = val.map(async (addr) => {
      let cand = new this.web3.eth.Contract(( this.CandidateABI  as any ), addr);
        return this.getAllDetails(cand);
    })
    this.allcand = await Promise.all(det);
  }

  async castVote(addr) {
    this.voteCasted = true;
    let val: any[] = await this.Manager.methods.getAllCandidates().call({from : this.head});
       
  }
}
