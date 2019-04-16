import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { SignInComponent } from './components/signin/signin.component';
import { SignUpComponent } from './components/signup/signup.component';
import { CreateVoterComponent } from './components/create-voter/create-voter.component';
import { CastVoteComponent } from './components/cast-vote/cast-vote.component';
const routes: Routes = [
  { path: 'createCandidate', component: SignInComponent },
  { path: 'createVoter', component: CreateVoterComponent },
  { path: 'castVote', component: CastVoteComponent },
  { path: 'signup', component: SignUpComponent }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
