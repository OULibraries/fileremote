<?php

/**
 * Minimal Dropbox file transfer class supporting just enough of the v2 HTTP API
 * to do chunked file transfers with Fine Uploader.
 */
class DropboxTransfer {

  /**
   * @param $auth
   * A Dropbox oauth token
   * @param $request
   * A FineUploader traditional endpoint request
   * @param $sessionid
   * A dropbox Session id
   * @param $gzclient
   * A Guzzle HTTP client
   * @param $chunk_path
   * Path to a chunk that we'll transfer
   * @param $remote_file
   * Dropbox path where we're trying to create a file.
   */


  function __construct($auth, $request,  $sessionid, $gzclient, $chunk_path, $remote_file) {
    $this->auth = $auth;
    $this->uuid = $request["qquuid"]; // transfer uuid assigned by fine uploader
    $this->offset = (int)$request["qqpartbyteoffset"]; // amount of data already sent
    $this->remote_file =  $remote_file;
    $this->totalparts = (int)$request["qqtotalparts"]; // total number of chunks
    $this->totalfilesize = (int)$request["qqtotalfilesize"]; // total data
    $this->partindex = (int)$request["qqpartindex"]; // index of this chunk
    $this->chunk_path = $chunk_path;
    $this->chunk_size = filesize($chunk_path);
    $this->chunk = fopen($chunk_path, "r");
    $this->gzclient = $gzclient;
    $this->sessionid = $sessionid;
  }


  /**
   * Perform a Dropbox file transfer command with the v2 HTTP API.
   *
   * See https://www.dropbox.com/developers/documentation/http/documentation
   *
   *  @param array $api_args
   *    The arguments required an Dropbox command.
   *  @param string $target
   *    An API endpoint URI for a dropbox command.
   */
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


  /**
   * Start a transfer session
   */
  function start() {
    $api_args = [
      "close"=> false
    ];
    $target = 'https://content.dropboxapi.com/2/files/upload_session/start';
    return $this->doTransfer( $api_args, $target);
  }

  /**
   * Append a chunk to an open transfer session
   */
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

  /**
   * Send final chunk and finalize transfer
   */
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

  /**
   * Do a simple file upload for small files
   */
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
