      function jsonArray(obj) {
            var res = []; 
            for(var i in obj){
              var row = [];
              for(var j in obj[i])
                row.push(parseInt(obj[i][j]));
            res.push(row);
            }
            //console.log(res);
            return res;
        }
