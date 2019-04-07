# A Blockchain based Biometric Verification Voting System

## Group Members: 

1.  Harsh Thakur - 160279
2.  Mayank Sharma - 160392
3.  Sahil Agarwal - 160605
4.  Shivank Garg - 160658
5.  Shreyaansh Gupta - 160664
6.  Adarsh Shrivastava - 150047

## The current problem with Voters’ verification


Presently, the voting system in campus is relies upon the manual verification by a person using his/her identity card. Moreover, this may not always be fair since we’re relying (trusting) on a third party belonging to the Election Commission. We propose to mitigate this problem by means of Biometrically Verified Voting System.

Biometric information (like fingerprints, Iris, etc.) can be stored on a blockchain and can be used to verify people for different purposes like voting, attendance, etc. Specifically, we’re using a fingerprint reader to store the fingerprints of a voter in a hashed form over the blockchain network. This ensures that even though some adversary may have access to the fingerprint data, he can’t misuse it owing to the hiding property of hash function.


## Technical Specifications:

We’re planning on using Ethereum for our contracts and we plan to deploy this whole project over the Ropsten Test Network. 

The contracts are as follows:

* A master contract, owned by the head of Election Commission - `ManagerContract`
* A contract identifying a person with his basic-credentials (non biometric ones) stored in it and accessible only by that person -  `VoterContract`
* A contract identifying each Candidate contesting in the Elections - `CandidateContract`
* A contract to count and keep tally of each vote in the Election - `VoteTallyContract`

### Phases of Voting

    

1.  **Creation of MasterContract:** The `MasterContract` is created using a wallet owned by the head of EC. This contract creates all the other contracts and keeps track of all the votes. Every vote which is made comes from this `MasterContract`. The code for this Contract is public and viewable by everyone.

2.  **Enrollment of Voters:** This is the phase when a person in signed up on the Blockchain Network and his credentials are stored in `VoterContract`. We’re currently assuming that we have a storage database which stores the private-keys associated with a wallet. In real-life scenarios, the private keys can be stored in the Institute ID card chip. The sole purpose of having these private keys is that it allows a person to later make changes to his credentials in-case they’re wrong initially.

3.  **Candidate Nominations:** A `CandidateContract` will be created by `ManagerContract` for candidates who are to eligible to file nomination in elections. This contract stores the credentials (achievements), alongwith the details of proposer and seconder. This also contains a publicly viewable Manifesto.

4.  **Voting Period:** A Voter can cast his/her vote using his fingerprint by going to a Poll Booth. Every computer allowed to vote has his public private key pair stored in the `MasterContract` and viewable only by the Head of EC. These computers have access to the `MasterContract` (with limits) and are allowed to only vote. The fingerprint is verified to ensure a person’s identity and the vote-counts and only be increased at-max by one per Voter.

5.  **Final Vote Count and Results:** Since, `MasterContract` stores the address of `CandidateContract`, we can have another mapping for each candidate which stores the candidate’s votes. Also, every increment in this mapping is only performed once a transaction has been made to the `VoteTallyContract` and that transaction has been approved into a block (a consensus has been reached).

For the biometric Fingerprint based verification, we’re planning on using a Fuzzy Hashing scheme to map a person’s finger minutiae to their VoterContract. We’re using Futronic’s Finger Print Reader, and the SDK is provided by the P-Club of IITK to interact with the reader.


## Salient Features

The major advantages of implementing a blockchain for this kind of application are:

* Identity claim is secured
* Consensus is taken care of by the Ethereum Network
* Every transaction involved is anonymous, meaning that it can’t be tracked back to a single person, yet we’re able to finally calculate the number of votes of a candidate.
* Highly scalable with the number of nodes and integrability with other application.
* No reliance on a single party for trust, given the mishappenings in Gymkhana Elections, 2019.

## Testing and Deployment

We wish to use truffle based tests for thoroughly testing the system on a local network before deploying to the Ropsten Test Network. Also, we’ve planned on performing full unit-testing for each Contract, and each function along with having integration tests at the end of this project.

Also, we have planned to open-source all this code along with contract code verification on EtherScan so that outside auditors can take a look and point out bugs in the code.

## Timeline


|Sr. No.  |Date |Planned Milestone Achieved|
|---|---|---|
| 1. | Apr, 8, 2019 | We’ve planned that we’ll finish writing the basic function, along with having a concrete design laid out for the whole system. | 
| 2. | Apr, 12, 2019 | Finish the complete functionalities, alongwith the fingerprint hashing. (Difficult) | We also plan to start writing unit-tests for each function and basic integration tests. |
| 3. | Apr, 14, 2019 | Finish writing tests. Perform integration testing and manual testing of the system. |
| 4 | Apr, 18, 2019 | Project Demo |

##  Present Caveats

The major challenge with implementing such a system, in a robust manner is the locality based hashes and their target space in which hash is produced. 

The scalability of this system might be limited by the fact that we’re using an approximate hash method, yet with the present techonology, this can’t be improved further because the Finger Print data is bound to have distortion and we can only have a certain threshold of matching with some other finger print already in database.

## Possible Future Improvements

Currently, we have assumed that we have a central store for storing a person’s private key. We may offload this functionality over to Insitute ID Card wherein we insert the person’s Private Key in the Chip found in the ID Card.

In such a setting, only the MasterContract can have access to this PrivateKey, that too in a very restrictive setting. This ensures security and prevents any other person (or an adversary) from gaining unfair access to someone else’s wallet.
