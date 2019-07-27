      function initMap() {
        var map = new google.maps.Map(document.getElementById('map'), {
          zoom: 12,
          center: {lat: 51.65, lng: 39.22},
          mapTypeId: 'terrain'
        });

        var Coordinates = [
          {lat: 51.659, lng: 39.228},
          {lat: 51.659, lng: 39.229},
          {lat: 51.660, lng: 39.229},
          {lat: 51.661, lng: 39.23}
        ];
        var myPath = new google.maps.Polyline({
          path: Coordinates,
          geodesic: true,
          strokeColor: '#FF0000',
          strokeOpacity: 1.0,
          strokeWeight: 2
        });

        myPath.setMap(map);
      }