$(document).ready( function() {

  var state = 0; // -1 is projects, 0 is normal, 1 is people
  var bIsSearch = false;


  var dataForSearch = [];

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

    data = sortByKey(data, 'project');
    for (var i = 0; i < data.length; i++){
      dataForSearch.push( data[i].project);
      dataForSearch.push( data[i].people);

      var p = document.createElement('div');
      p.classList.add('project');
      p.innerHTML = '<h3>' + data[i].project + '</h3><h4>' + data[i].people + '</h4>';
      p.setAttribute('data-project', data[i].project);
      p.setAttribute('data-people', data[i].people);
      p.setAttribute('data-slug', data[i].slug);
      $('#project-list').append( p );

      var n = document.createElement('div');
      n.classList.add('person');
      n.innerHTML = '<h3>' + data[i].people + '</h3><h4>' + data[i].project + '</h4>';
      n.setAttribute('data-project', data[i].project);
      n.setAttribute('data-people', data[i].people);
      n.setAttribute('data-slug', data[i].slug);
      $('#people-list').append( n );      

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


  //Search
  $('#search-input').autocomplete({
    minLength: 2,
    source: function(request, response) {
      var results = $.ui.autocomplete.filter(dataForSearch, request.term);
      response(results.slice(0, 3));
    }
  });

  $('#search-input').blur(function() {
    console.log( $('#search-input').val() );

    //send socket info from val
  });


  /**************************************************************************************/
  /**********************************SWIPE/DRAG EVENTS***********************************/
  /**************************************************************************************/

  var optionsX = {
    dragLockToAxis: true,
    dragBlockHorizontal: true,
  };
  var hammertimeX = new Hammer(document, optionsX);


  hammertimeX.on("swiperight", function(event) {
    event.gesture.preventDefault();

    if (!bIsSearch) {
      $('#search-input').blur();
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
    }
  });

  hammertimeX.on("swipeleft", function(event) {
    event.gesture.preventDefault();

    if (!bIsSearch) {
      $('#search-input').blur();
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
    }
  });

  var optionsY = {
    dragLockToAxis: true,
  };
  var hammertimeY = new Hammer(document, optionsY);

  hammertimeY.on("swipedown", function(event) {
    
    if ( event.gesture.center.clientY < 300 && state == 0) {
      $('#search').removeClass('search-hidden').addClass('search-visible');
      bIsSearch = true;
      $('#search-input').focus();
      console.log('swipedown');
    }
  });

  hammertimeY.on("swipeup", function(event) {
      $('#search').removeClass('search-visible').addClass('search-hidden');
      bIsSearch = false;
      $('#search-input').blur().val('');
      console.log('swipeup');
  });

  // $('#project-list').scroll( function(event) {
  //   event.preventDefault();
  //   console.log('scroll');
  // });

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
