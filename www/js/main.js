$(document).ready( function() {

  var state = 0; // -1 is projects, 0 is normal, 1 is people


  //Initialize socket and get data

  var socket = io.connect('http://' + window.location.hostname + ':8080');
  socket.on('connect', function() {
    console.log('connected!')
  });

  socket.on('init', function (data) {
    createContent(data);
  });

  //Create content from data on page

  function createContent(data) {
    console.log(data);

    for (var i = 0; i < data['projects'].length; i++){
      var p = document.createElement('div');
      p.classList.add('project');
      p.innerHTML = data['projects'][i];
      $('#project-list').append( p );
    }

    for (var i = 0; i < data['people'].length; i++){
      var p = document.createElement('div');
      p.classList.add('person');
      p.innerHTML = data['people'][i];
      $('#people-list').append( p );
    }
  }

  //Send data to server if project/person is selected.
  $('.project').on('click', function(event) {
    alert('hi');
  });

  $('.project').click( function(evt){
    console.log('hi');
  });


  /**************************************************************************************/
  /**********************************SWIPE/DRAG EVENTS***********************************/
  /**************************************************************************************/

  var options = {
    dragLockToAxis: true,
    dragBlockHorizontal: true,
    preventDefault: true
  };
  var hammertime = new Hammer(document, options);

  hammertime.on("swiperight", function(event) {

      if (state == -1) {
        //do nothing
      }
      else if (state == 0) {
        $('#content').css('overflow', 'scroll');
        $('header').removeClass('header-visible').addClass('header-left');
        $('#project-list').removeClass('projects-hidden').addClass('projects-visible');
        state = -1;
      }
      else if (state == 1) {
        $('#content').scrollTop( 0 );
        $('header').removeClass('header-right').addClass('header-visible');
        $('#people-list').removeClass('people-visible').addClass('people-hidden');
        state = 0;
      }


      console.log('swiperight');
  });

  hammertime.on("swipeleft", function(event) {

      if (state == -1) {
        $('#content').scrollTop( 0 );
        $('header').removeClass('header-left').addClass('header-visible');
        $('#project-list').removeClass('projects-visible').addClass('projects-hidden');
        state = 0;
      }
      else if (state == 0) {
        $('header').removeClass('header-visible').addClass('header-right');
        $('#people-list').removeClass('people-hidden').addClass('people-visible');
        state = 1;
      }
      else if (state == 1) {
        //do nothing
      }

      console.log('swipeleft');
  });

  hammertime.on("swipedown", function(event) {

      $('#search').removeClass('search-hidden').addClass('search-visible');

      console.log('swipedown');
  });

  hammertime.on("swipeup", function(event) {

      $('#search').removeClass('search-visible').addClass('search-hidden');

      console.log('swipedown');
  });

  /**************************************************************************************/
  /**************************************************************************************/
  /**************************************************************************************/


});
