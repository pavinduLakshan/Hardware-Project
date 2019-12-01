import React, { useState,useEffect } from "react";
import ReactTable from "react-table";
import "react-table/react-table.css";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import Button from "react-bootstrap/Button";
import Axios from "axios";

const columns = [
  {
    Header: "Id",
    accessor: "fin_id",
    width: 100,
  },
  {
    Header: "Name",
    accessor: "name",
  },
  {
    Header: "Contact",
    accessor: "p_mobile",
    width: 350,
  },
];

const Table = () => {
  const [students, setStudents] = useState([]);
  const [id, setId] = useState(null);

  useEffect( () => {
    Axios.get('/students').then(
      res => {
        console.log(res.data)
        setStudents(res.data)
      }
    ).catch(err => console.log(err))
  },[])

  return (
    <div style={{padding: 0,margin:0}}>
      <Row style={{padding: 0,margin:0}}>
        <Col md={6} >
          <h2 style={{margin: "2%"}}>ABC Institute</h2>
        </Col>
        <Col md={6}>
          <a href={"/form?id=" + id}>
            <Button variant="primary" disabled={id ? false : true} style={{ float: "right", margin: "2%" }}>
              Update Record
            </Button>
          </a>
        </Col>
      </Row>
      <ReactTable
        style={{ marginLeft: "1%", marginRight: "1%" }}
        data={students}
        columns={columns}
        defaultPageSize={10}
        showPageSizeOptions={false}
        getTrProps={(state, rowInfo) => {
          if (rowInfo && rowInfo.row) {
            return {
              onClick: () => {
                setId(rowInfo.original.fin_id)
              },
              style: {
                color: rowInfo.original.fin_id === id ? 
                '#09C442' : 
                'black'
              }
            }
          }else{
            return {}
          }
        }}
      />
    </div>
  );
};

export default Table;
