import { ADDRESS } from '../signin/signin.component';
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
  selector: 'app-create-voter',
  templateUrl: './create-voter.component.html',
  styleUrls: ['./create-voter.component.css']
})
export class CreateVoterComponent implements OnInit {
  public form: FormGroup;
  public accounts;
  public head;
  public v; public c; public outsider;
  public Manager;
  public Candidate;
  public allcand;

  private ManagerABI;
  private VoterABI;

  public selectedAddr;
  backend = 'http://localhost:8000/';
  constructor(
    private fb: FormBuilder,
    private http: HttpClient,
    @Inject(WEB3) private web3: Web3
  ) { }

  async ngOnInit() {

    this.form = this.fb.group({
      addr: ['', Validators.required ],
      name: ['', Validators.required ],
      papa: ['', Validators.required ],
      mama: ['', Validators.required ],
      hall: ['', Validators.required ],
      rollNo: ['', Validators.required ],
      dob: ['']
    })

    this.accounts = await this.web3.eth.getAccounts();
    this.ManagerABI = await this.http.get(this.backend + 'abi/Manager.json').toPromise();
    this.VoterABI = await this.http.get(this.backend + 'abi/Voter.json').toPromise();

    this.Manager = new this.web3.eth.Contract(( this.ManagerABI  as any ), ADDRESS);
    console.log( this.Manager.address );

    this.head = this.accounts[0];
    this.outsider = {
      first: this.accounts[7],
      second: this.accounts[8],
      third: this.accounts[9]
    };
  }
  updateAddr(foobar : string) {
    this.selectedAddr = foobar;
    this.form.controls['addr'].setValue(this.selectedAddr);
    console.log(this.selectedAddr);
  }

  async createVoter() {
    let personAddr = this.form.value.addr;
    let name = this.form.value.name;
    let mama = this.form.value.mama;
    let papa = this.form.value.papa;
    let hall = this.form.value.hall;
    let rollNo = Number.parseInt( this.form.value.rollNo );
    let dob = Date.now();

    let val = await this.Manager.methods.createVoter(
      this.form.value.addr ,
      name,
      papa,
      mama,
      hall,
      rollNo,
      dob
    ).send({from : this.head}, (err, txHash) => {
      console.log(err);
      console.log(txHash);
    });
  }
}
