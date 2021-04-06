// Imports the server.js file to be tested.
let server = require("../server");
//Assertion (Test Driven Development) and Should, Expect(Behaviour driven development) library
let chai = require("chai");
// Chai HTTP provides an interface for live integration testing of the API's.
let chaiHttp = require("chai-http");
const { response } = require("express");
const { name } = require("ejs");
chai.should();
chai.use(chaiHttp); 
const { expect } = chai;
var assert = chai.assert;




describe("Server!", () => {

    // Sample test case given to test / endpoint. 
    it("Returns the default welcome message", done => {
      chai
        .request(server)
        .get("/")
        .end((err, res) => {
          expect(res).to.have.status(200);
          expect(res.body.status).to.equals("success");
          assert.strictEqual(res.body.message, "Welcome!");
          done();
        });
    });

    // Please add your test cases here.
    it("Returns a list of operations", done => {
      chai  
        .request(server)
        .get("/operations")
        .end((err, response) => {
          assert.isNotEmpty(response.body);
          assert.isArray(response.body);
          done();
        });
    });
    it("Returns the details of an operation", done => {
      chai  
        .request(server)
        .get("/operations/1")
        .end((err, response) => {
          assert.equal(response.body.id, 1);
          assert.equal(response.body.name, 'Add');
          assert.equal(response.body.sign, '+');
          done();
        });
    });
    it("Posts the details of an operation", done => {
      chai  
        .request(server)
        .post("/operations")
        .send({id: 4, name: 'Divide', sign: '/'})
        .end((err, response) => {
          expect(response.body).to.have.property('id', 4);
          expect(response.body).to.have.property('name', 'Divide');
          expect(response.body).to.have.property('sign', '/');
          done();
        });
    });
  });