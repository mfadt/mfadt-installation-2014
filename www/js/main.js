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
    // console.log(data);

    data = sortByKey(data, 'project');
    for (var i = 0; i < data.length; i++){
      var p = document.createElement('div');
      p.classList.add('project');
      p.innerHTML = data[i].project;
      p.setAttribute('data-project', data[i].project);
      p.setAttribute('data-people', data[i].people);
      p.setAttribute('data-slug', data[i].slug);
      $('#project-list').append( p );
    }

    data = sortByKey(data, 'people')
    for (var i = 0; i < data.length; i++){
      var p = document.createElement('div');
      p.classList.add('person');
      p.innerHTML = data[i].people;
      p.setAttribute('data-project', data[i].project);
      p.setAttribute('data-people', data[i].people);
      p.setAttribute('data-slug', data[i].slug);
      $('#people-list').append( p );
    }
  }

  //Send data to server if project/person is selected.

  $('body').on('touch', '.project', function(){
    var data = {
      project: $(this).attr('data-project'), 
      people: $(this).attr('data-people'),
      slug: $(this).attr('data-slug')
    };
    
    socket.emit('load_model', data);
  });

  $('body').on('touch', '.person', function(){
    var data = {
      project: $(this).attr('data-project'), 
      people: $(this).attr('data-people'),
      slug: $(this).attr('data-slug')
    };

    socket.emit('load_model', data);
  });

  $('something').click(function() {

  })



  /**************************************************************************************/
  /**********************************SWIPE/DRAG EVENTS***********************************/
  /**************************************************************************************/

  var options = {
    dragLockToAxis: true,
    dragBlockHorizontal: true,
    preventDefault: false
  };
  var hammertime = new Hammer(document, options);

  hammertime.on("touch", function(event) {
    // alert(event);
  });

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
  /************************************** HELPERS ***************************************/
  /**************************************************************************************/

  function sortByKey(array, key) {
    return array.sort(function(a, b) {
      var x = a[key];
      var y = b[key];

      if (typeof x == "string")
      {
          x = x.toLowerCase(); 
          y = y.toLowerCase();
      }

      return ((x < y) ? -1 : ((x > y) ? 1 : 0));
    });
  }

});
