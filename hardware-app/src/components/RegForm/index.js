import React,{ useState, useEffect} from "react";
import Form from "react-bootstrap/Form";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import {Link} from 'react-router-dom'
import Axios from 'axios'
import Button from "react-bootstrap/Button";
import InputGroup from "react-bootstrap/InputGroup";

const RegForm = props => {
  const [id,setId] = useState("")
  const [name,setName] = useState("")
  const [mobile,setMobile] = useState("")
  
  useEffect(() => {
    const params = new URLSearchParams(window.location.search);
    const id = params.get("id");
    if(id === null){
      alert("No student id is available")
      props.history.push('/')
    }
    setId(id)
  },[props.history]);

  function updateStudent(event){
    event.preventDefault()
    const fin_id = parseInt(id)
    const p_mobile = "+94" + mobile.slice(1,10)
    console.log({fin_id,name, mobile})
    Axios.put('/students/'+fin_id,{
      name,p_mobile
    }).then(res => {
      console.log(res.data)
      alert("Successfully updated")
    }).catch(err => alert("Error occured"))
  }

  return (
    <Row>
      <Col md={{ span: 6, offset: 3 }} style={{}}>
        <Form onSubmit={updateStudent}>
          <h2 style={{ textAlign: "center" }}>Update Form</h2>
          <Form.Group>
            <label />
            <InputGroup style={{ width: "30%" }}>
              <InputGroup.Prepend>
                <InputGroup.Text id="inputGroupPrepend">
                  Student Id
                </InputGroup.Text>
              </InputGroup.Prepend>
              <Form.Control
                readOnly
                defaultValue={id}
                aria-describedby="inputGroupPrepend"
                required
              />
            </InputGroup>
          </Form.Group>

          <Form.Group>
            <label />
            <InputGroup>
              <InputGroup.Prepend>
                <InputGroup.Text id="inputGroupPrepend">Name</InputGroup.Text>
              </InputGroup.Prepend>
              <Form.Control
                type="text"
                placeholder="Name"
                name="name"
                defaultValue={name}
                onChange={e => setName(e.target.value)}
                aria-describedby="inputGroupPrepend"
                required
              />
            </InputGroup>
          </Form.Group>

          <Form.Group>
            <label />
            <InputGroup>
              <InputGroup.Prepend>
                <InputGroup.Text id="inputGroupPrepend">
                  Contact
                </InputGroup.Text>
              </InputGroup.Prepend>
              <Form.Control
                type="text"
                placeholder="Contact"
                maxLength={10}
                name="p_mobile"
                defaultValue={mobile}
                onChange={e => setMobile(e.target.value)}
                aria-describedby="inputGroupPrepend"
                required
              />
            </InputGroup>
          </Form.Group>

          <Form.Group>
            <Button type="submit" variant="success" style={{ width: "100%" }}>
              Update Record
            </Button>
          </Form.Group>
        </Form>
        <div style={{display: "flex", justifyContent: "center"}}>
        <Link to="/">Back to dashboard</Link>
        </div>
      </Col>
    </Row>
  );
};

export default RegForm;
