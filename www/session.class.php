<?php
class Session {

	public function	__construct() {
		session_start();
	}

	public function setFlash( $message, $type = 'error' ) {
		if (!isset($_SESSION['flash']))
			$_SESSION['flash'] = array();

		array_push($_SESSION['flash'], array(
			'message' => $message,
			'type' => $type,
			)
		);
	}

	public function flash() {
		if (isset($_SESSION['flash'])) {
			$count = count($_SESSION['flash']);
			for ($i = 0; $i < $count; $i++) {
				?>
				<div class="alert alert-<?php echo $_SESSION['flash'][$i]['type'];?>">
					<p>
						<?php echo $_SESSION['flash'][$i]['message']; ?>
					</p>
				</div>
				<?php
				unset($_SESSION['flash'][$i]);
			}
			unset($_SESSION['flash']);
		}
	}
}
