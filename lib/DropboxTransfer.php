<?php



class DropboxTransfer {

  function __construct($auth, $request,  $sessionid, $gzclient, $chunk_path, $remote_file) {
    $this->auth = $auth;
    $this->uuid = $request["qquuid"];
    $this->offset = (int)$request["qqpartbyteoffset"];
    $this->remote_file =  $remote_file;
    $this->totalparts = (int)$request["qqtotalparts"];
    $this->totalfilesize = (int)$request["qqtotalfilesize"];
    $this->partindex = (int)$request["qqpartindex"];
    $this->chunk_path = $chunk_path;
    $this->chunk_size = filesize($chunk_path);
    $this->chunk = fopen($chunk_path, "r");
    $this->gzclient = $gzclient;
    $this->sessionid = $sessionid;
  }


  function doTransfer( $api_args, $target) {

    $request = $this->gzclient->createRequest('POST', $target);
    $request->setHeader("Authorization", "Bearer {$this->auth} ");
    $request->setHeader("Dropbox-API-Arg", json_encode($api_args));
    $request->setHeader("Content-Type", "application/octet-stream");
    $request->setBody(GuzzleHttp\Stream\Stream::factory($this->chunk));

    $result = new stdClass();

    try {
      /* Happy Path */
      $response = $this->gzclient->send($request);
      $result->status = $response->getStatusCode();
      $result->info = json_decode( $response->getBody() );
      $result->success = true;
    }catch (GuzzleHttp\Exception\ClientException $e ) {
      /* Normal errors */
      $result->status = $e->getResponse()->getStatusCode();
      $result->reason = $e->getResponse()->getReasonPhrase();
      $result->success = false;
      $result->error = json_decode( $e->getResponse()->getBody() );
    }catch(Exception $e) {
      /* Badness! */
      $result->error = $e->getMessage();
      $result->success = false;
    }

    return $result;
  }


  function start() {
    $api_args = ["close"=> false];
    $target = 'https://content.dropboxapi.com/2/files/upload_session/start';
    return $this->doTransfer( $api_args, $target);
  }

  function appendv2() {
    $api_args = [
      "close"=> false,
      "cursor" => [
        "session_id" => $this->sessionid,
        "offset" => $this->offset
      ]
    ];

    $target= 'https://content.dropboxapi.com/2/files/upload_session/append_v2';

    return $this->doTransfer($api_args, $target);
  }


  function finish() {
    $api_args = [
      "cursor" => [
        "session_id" =>$this->sessionid,
        "offset" => $this->offset
      ],
      "commit" => [
        "path"=> "/".$this->remote_file ,
        "mode"=> "add",
        "autorename"=> true,
        "mute"=> false
      ]
    ];
    $target='https://content.dropboxapi.com/2/files/upload_session/finish';
    return $this->doTransfer($api_args, $target);
  }


  function upload() {
    $api_args = [
      "path"=> "/".$this->remote_file ,
      "mode"=> "add",
      "autorename"=> true,
      "mute"=> false
    ];
    $target='https://content.dropboxapi.com/2/files/upload';
    return $this->doTransfer($api_args, $target);
  }

}
