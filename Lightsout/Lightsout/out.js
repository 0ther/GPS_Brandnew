      function initMap() {
        var map = new google.maps.Map(document.getElementById('map'), {
          zoom: 12,
center: {lat: 51.65, lng: 39.22},
          mapTypeId: 'terrain'
        });

        var Coordinates = [
{lat: 51.659, lng: 39.227},
{lat: 51.659, lng: 39.228},
{lat: 51.660, lng: 39.228},
{lat: 51.660, lng: 39.229},
{lat: 51.660, lng: 39.230},
{lat: 51.661, lng: 39.230},
{lat: 51.661, lng: 39.231},
{lat: 51.661, lng: 39.232},
{lat: 51.662, lng: 39.232},
{lat: 51.662, lng: 39.233},
{lat: 51.662, lng: 39.234},
{lat: 51.663, lng: 39.234},
{lat: 51.664, lng: 39.234},
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
