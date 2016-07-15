.. -*- rst -*-

===============
 OS-OAUTH1 API
===============

Provide the ability for identity users to delegate roles to third party
consumers via the `OAuth 1.0a specification <http://oauth.net/core/1.0a/>`__.
Requires v3.0+ of the Identity API. An OAuth-derived token will provide
a means of acting on behalf of the authorizing user.

Overview
========

Definitions
-----------

- *User:* An Identity API service user, the entity whose role(s) will be
  delegated, and the entity that authorizes Request Tokens.

- *Request Token:* A token used by the Consumer to obtain authorization from
  the User, and exchanged with an OAuth Verifier for an Access Token.

- *Access Token:* A token used by the Consumer to request new Identity API
  tokens on behalf of the authorizing User, instead of using the User’s
  credentials.

- *Token Key:* A key used by the token to identify itself. Both Request Tokens
  and Access Tokens have Token Keys. For OpenStack purposes, the Token Key is
  the Token ID.

- *Token Secret:* A secret used by the Consumer to establish ownership of a
  given Token. Both Request Tokens and Access Tokens have Token Secrets.

- *OAuth Verifier:* A string that must be provided with the corresponding
  Request Token in exchange for an Access Token.

Delegated Authentication Flow
-----------------------------

Delegated Authentication via OAuth is done in five steps:

#. An Identity API service User `creates a Consumer
   <#create-consumer>`__.

#. The Consumer `obtains an unauthorized Request Token
   <#create-request-token>`__.

#. The User `authorizes the Request Token
   <#authorize-request-token>`__.

#. The Consumer `exchanges the Request Token for an Access Token
   <#create-access-token>`__.

#. The Consumer `uses the Access Token to request an Identity API service Token
   <#authenticate-with-identity-api>`__.


Create consumer
===============

.. rest_method::  POST /v3/OS-OAUTH1/consumers

Enables a user to create a consumer.

Normal response codes: 201
Error response codes: 413, 415, 405, 404, 403, 401, 400, 503, 409

Request
-------

.. rest_parameters:: parameters.yaml

    - description: description

Request Example
---------------

.. literalinclude:: ./samples/OS-OAUTH1/consumers-create-request.json
   :language: javascript


Delete consumer
===============

.. rest_method::  DELETE /v3/OS-OAUTH1/consumers/{consumer_id}

Deletes a consumer.

When you delete a consumer, any associated request tokens, access
tokens, and Identity API tokens are also deleted.

Normal response codes: 204
Error response codes: 413, 415, 405, 404, 403, 401, 400, 503, 409

Request
-------

.. rest_parameters:: parameters.yaml

   - consumer_id: consumer_id


List consumers
==============

.. rest_method::  GET /v3/OS-OAUTH1/consumers

Lists consumers.

Normal response codes: 200
Error response codes: 413, 405, 404, 403, 401, 400, 503

Response Example
----------------

.. literalinclude:: ./samples/OS-OAUTH1/consumers-list-response.json
   :language: javascript


Show consumer details
=====================

.. rest_method::  GET /v3/OS-OAUTH1/consumers/{consumer_id}

Shows details for a consumer.

Normal response codes: 200
Error response codes: 413, 405, 404, 403, 401, 400, 503

Request
-------

.. rest_parameters:: parameters.yaml

   - consumer_id: consumer_id

Response Example
----------------

.. literalinclude:: ./samples/OS-OAUTH1/consumer-show-response.json
   :language: javascript


Update consumer
===============

.. rest_method::  PATCH /v3/OS-OAUTH1/consumers/{consumer_id}

Updates the description for a consumer.

If you try to update any attribute other than description, the HTTP
400 Bad Request error is returned.

Normal response codes: 200
Error response codes: 413, 415, 405, 404, 403, 401, 400, 503, 409

Request
-------

.. rest_parameters:: parameters.yaml

   - consumer_id: consumer_id

Request Example
---------------

.. literalinclude:: ./samples/OS-OAUTH1/consumer-update-request.json
   :language: javascript

Response Example
----------------

.. literalinclude:: ./samples/OS-OAUTH1/consumer-update-response.json
   :language: javascript

Create request token
====================

.. rest_method::  POST /v3/OS-OAUTH1/request_token

Enables a consumer to get an unauthorized request token.

Supported signature methods: HMAC-SHA1.

The consumer must provide all required OAuth parameters in the
request. See `Consumer Obtains a Request Token
<http://oauth.net/core/1.0a/#auth_step1>`_.

Normal response codes: 200
Error response codes: 413, 415, 405, 404, 403, 401, 400, 503, 409

Request
-------

.. rest_parameters:: parameters.yaml

    - requested_project_id: requested_project_id

Response Example
----------------

.. literalinclude:: ./samples/OS-OAUTH1/request-token-create-response.txt
   :language: javascript


Authorize request token
=======================

.. rest_method::  POST /v3/OS-OAUTH1/authorize/{request_token_id}

To authorize the Request Token, the authorizing user must have access to the
requested project. Upon successful authorization, an OAuth Verifier code is
returned. The Consumer receives the OAuth Verifier from the User out-of-band.

Relationship: ``http://docs.openstack.org/api/openstack-identity/3/ext/OS-OAUTH1/1.0/rel/authorize_request_token``

Normal response codes: 200
Error response codes: 413, 415, 405, 404, 403, 401, 400, 503, 409

Request
-------

.. literalinclude:: ./samples/OS-OAUTH1/authorize-request-token-request.json
   :language: javascript

Response Example
----------------

.. literalinclude:: ./samples/OS-OAUTH1/authorize-request-token-response.json
   :language: javascript

Create access token
===================

.. rest_method::  POST /v3/OS-OAUTH1/access_token

Enables a consumer to create an access token by exchanging a request token for an access token.

After the user authorizes the request token, the consumer exchanges
the authorized request token and OAuth verifier for an access
token.

Supported signature methods: HMAC-SHA1.

The consumer must provide all required OAuth parameters in the
request. See `Consumer Obtains a Request Token
<http://oauth.net/core/1.0a/#auth_step1>`_.

Supported signature methods: HMAC-SHA1.

You must provide all required OAuth parameters in the request. See
`Consumer Obtains a Request Token
<http://oauth.net/core/1.0a/#auth_step1>`_.

Normal response codes: 200
Error response codes: 413, 415, 405, 404, 403, 401, 400, 503, 409

Response Example
----------------

.. literalinclude:: ./samples/OS-OAUTH1/access-token-create-response.txt
   :language: javascript


Get authorized access token
===========================

.. rest_method::  GET /v3/OS-OAUTH1/users/{user_id}/access_tokens/{access_token_id}

Gets an authorized access token.

Normal response codes: 200
Error response codes: 413, 405, 404, 403, 401, 400, 503

Request
-------

.. rest_parameters:: parameters.yaml

   - user_id: user_id
   - access_token_id: access_token_id

Response Example
----------------

.. literalinclude:: ./samples/OS-OAUTH1/access-token-show-response.json
   :language: javascript


Revoke access token
===================

.. rest_method::  DELETE /v3/OS-OAUTH1/users/{user_id}/access_tokens/{access_token_id}

Enables a user to revoke an access token, which prevents the consumer from
requesting new Identity Service API tokens. Also, revokes any Identity Service
API tokens that were issued to the consumer through that access token.

Normal response codes: 204
Error response codes: 413, 415, 405, 404, 403, 401, 400, 503, 409

Request
-------

.. rest_parameters:: parameters.yaml

   - user_id: user_id
   - access_token_id: access_token_id


List authorized access tokens
=============================

.. rest_method::  GET /v3/OS-OAUTH1/users/{user_id}/access_tokens

Lists authorized access tokens.

Normal response codes: 200
Error response codes: 413, 405, 404, 403, 401, 400, 503

Request
-------

.. rest_parameters:: parameters.yaml

   - user_id: user_id

Response Example
----------------

.. literalinclude:: ./samples/OS-OAUTH1/access-tokens-list-response.json
   :language: javascript


List roles for an access token
==============================

.. rest_method::  GET /v3/OS-OAUTH1/users/{user_id}/access_tokens/{access_token_id}/roles

Lists associated roles for an access token.

Normal response codes: 200
Error response codes: 413, 415, 405, 404, 403, 401, 400, 503

Request
-------

.. rest_parameters:: parameters.yaml

   - user_id: user_id
   - access_token_id: access_token_id


Show role details for an access token
=====================================

.. rest_method::  GET /v3/OS-OAUTH1/users/{user_id}/access_tokens/{access_token_id}/roles/{role_id}

Shows details for a role for an access token.

Normal response codes: 200
Error response codes: 413, 415, 405, 404, 403, 401, 400, 503

Request
-------

.. rest_parameters:: parameters.yaml

   - user_id: user_id
   - role_id: role_id
   - access_token_id: access_token_id


Authenticate with Identity API
==============================

.. rest_method::  POST /v3/auth/tokens

Enables a consumer to get an Identity Service authentication token.

The token represents the delegated authorization and identity
(impersonation) of the authorizing user. The roles and scope of the
generated token match those that the consumer initially requested.

Supported signature methods: HMAC-SHA1.

The consumer must provide required OAuth parameters in the request.
See `Consumer Obtains a Request Token
<http://oauth.net/core/1.0a/#auth_step1>`_.

The returned token is scoped to the requested project and with the
requested roles. In addition to the standard token response, the
token has an OAuth-specific object.

Example OAuth-specific object in a token:

.. code-block:: json

   "OS-OAUTH1": {
       "access_token_id": "cce0b8be7"
   }

Normal response codes: 200
Error response codes: 413, 405, 404, 403, 401, 400, 503