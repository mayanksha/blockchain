import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { NgModule } from '@angular/core';
import { HttpClientModule } from '@angular/common/http';

import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { FlexLayoutModule } from '@angular/flex-layout';
import { AppComponent } from './app.component';
import { SignInComponent } from './components/signin/signin.component';
import { AppRoutingModule } from './app-routing.module';

import {
  MatIconModule,
  MatToolbarModule,
  MatButtonModule,
  MatAutocompleteModule,
  MatInputModule,
  MatGridListModule,
  MatExpansionModule,
  MatCardModule,
  MatMenuModule,
  MatSidenavModule,
  MatSelectModule,
  MatRadioModule,
  MatDialogModule,
  MatProgressSpinnerModule,
  MatTabsModule,
  MatCheckboxModule
} from '@angular/material';
import { SignUpComponent } from './components/signup/signup.component';
import { CreateVoterComponent } from './components/create-voter/create-voter.component';
import { TopbarComponent } from './components/topbar/topbar.component';
import { CastVoteComponent } from './components/cast-vote/cast-vote.component';
import { ViewVotesComponent } from './components/view-votes/view-votes.component';
@NgModule({
  declarations: [
    AppComponent,
    SignInComponent,
    SignUpComponent,
    CreateVoterComponent,
    TopbarComponent,
    CastVoteComponent,
    ViewVotesComponent,
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule,
    ReactiveFormsModule,
    AppRoutingModule,
    HttpClientModule,
    FlexLayoutModule,
    MatIconModule,
    MatToolbarModule,
    MatButtonModule,
    MatAutocompleteModule,
    MatInputModule,
    FormsModule,
    MatGridListModule,
    MatExpansionModule,
    MatCardModule,
    MatMenuModule,
    MatSidenavModule,
    MatSelectModule,
    MatRadioModule,
    MatDialogModule,
    MatProgressSpinnerModule,
    MatTabsModule,
    MatCheckboxModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})

export class AppModule {}
