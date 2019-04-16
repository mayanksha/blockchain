import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ViewVotesComponent } from './view-votes.component';

describe('ViewVotesComponent', () => {
  let component: ViewVotesComponent;
  let fixture: ComponentFixture<ViewVotesComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ViewVotesComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ViewVotesComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
